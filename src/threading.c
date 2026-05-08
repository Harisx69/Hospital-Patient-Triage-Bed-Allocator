#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include "semaphores.h"
#include <sys/wait.h>
#include "logger.h"
#include "statistics.h"
#include "threading.h"
#include "queue.h"
#include "memory_manager.h"
#include "ipc.h"
pthread_mutex_t queue_mutex;
pthread_mutex_t ward_mutex;
pthread_cond_t queue_not_empty;

void* receptionist_thread(void* arg)
{
    PatientQueue* queue;

    queue = (PatientQueue*)arg;

    while(runner)
    {
        PatientRecord p;

        int bytes;

        bytes = read(pipe_fd[0],
                     &p,
                     sizeof(PatientRecord));

        if(bytes <= 0)
        {
            continue;
        }

        sem_wait(&queue_empty);

        pthread_mutex_lock(&queue_mutex);

        enqueue_patient(queue, p);

        printf("Patient %d added from triage\n",
               p.patient_id);

        sem_post(&queue_full);

        pthread_cond_signal(&queue_not_empty);

        pthread_mutex_unlock(&queue_mutex);
    }

    return NULL;
}

void* scheduler_thread(void* arg)
{
    
    PatientQueue* queue = (PatientQueue*)arg;

    while(runner) {
        sem_wait(&queue_full);
        sem_wait(&queue_full);
        pthread_mutex_lock(&queue_mutex);

        while(is_queue_empty(queue))
        {
            pthread_cond_wait(&queue_not_empty,
                              &queue_mutex);
        }

        PatientRecord p = dequeue_patient(queue);
        static int current_time = 0;

int burst = rand() % 5 + 1;

int start_time = current_time;

int end_time = current_time + burst;

fcfs_log(p,
         start_time,
         end_time);

current_time = end_time;
        sem_post(&queue_empty);
        sem_post(&queue_empty);
        pthread_mutex_unlock(&queue_mutex);

        pthread_mutex_lock(&ward_mutex);
        if(strcmp(p.name, "ICU") == 0)
{
    sem_wait(&icu_sem);
}

if(strcmp(p.name, "ISOLATION") == 0)
{
    sem_wait(&isolation_sem);
}
        int bed = best_fit_allocate(p);

        pthread_mutex_unlock(&ward_mutex);
        total_admitted++;

char msg[100];

sprintf(msg,
        "Patient %d got bed %d",
        p.patient_id,
        bed);

write_log("logs/hospital.log",
          msg);
        if(bed == -1)
        {
            printf("No bed for patient %d\n",
                   p.patient_id);

            continue;
        }

        pid_t pid = fork();

        if(pid == 0)
        {
            char id[20];
            char bedid[20];

            sprintf(id, "%d", p.patient_id);
            sprintf(bedid, "%d", bed);

            char* args[] = {
                "./patient_simulator",
                id,
                bedid,
                NULL
            };

            execv("./patient_simulator", args);
        }
    }

    return NULL;
}

void* discharge_listener_thread(void* arg)
{
    int fd;

    fd = open(DISCHARGE_FIFO, O_RDONLY);

    while(runner){
        int patient_id;

        int bytes;

        bytes = read(fd,
                     &patient_id,
                     sizeof(int));

        if(bytes > 0)
        {
            pthread_mutex_lock(&ward_mutex);

            free_partition(patient_id);
            if(patient_id % 2 == 0)
{
    sem_post(&icu_sem);
}
else
{
    sem_post(&isolation_sem);
}

            pthread_mutex_unlock(&ward_mutex);
            total_discharged++;

char msg[100];

sprintf(msg,
        "Patient %d discharged",
        patient_id);

write_log("logs/hospital.log",
          msg);
            printf("Patient %d discharged\n",
                   patient_id);

            print_ward_map();
        }
    }

    return NULL;
}

void* monitor_thread(void* arg)
{
    PatientQueue* queue = (PatientQueue*)arg;

    while(runner){
        sleep(5);

        pthread_mutex_lock(&ward_mutex);

        printf("\n===== MONITOR =====\n");

        print_ward_map();

        fragmentation_report();

        pthread_mutex_unlock(&ward_mutex);

        pthread_mutex_lock(&queue_mutex);

        printf("Queue Size: %d\n",
               queue_size(queue));
        display_queue(queue);

        pthread_mutex_unlock(&queue_mutex);
    }

    return NULL;
}