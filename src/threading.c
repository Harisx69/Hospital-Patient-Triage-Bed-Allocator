#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
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
    PatientQueue* queue = (PatientQueue*)arg;

    int id = 10;

    while(runner) {
        sleep(3);

        PatientRecord p;

        p.patient_id = id;
        p.severity = rand() % 10 + 1;
        if(p.severity >= 9) p.priority = 1;
        else if(p.severity >= 7)p.priority = 2;
        else if(p.severity >= 5) p.priority = 3;
        else if(p.severity >= 3) p.priority = 4;
        else  p.priority = 5;

        p.care_units = 1;
        p.arrival_time = time(NULL);

        pthread_mutex_lock(&queue_mutex);

        enqueue_patient(queue, p);

printf("Patient %d arrived | Severity: %d | Priority: %d\n",
       id,
       p.severity,
       p.priority);

        pthread_cond_signal(&queue_not_empty);

        pthread_mutex_unlock(&queue_mutex);

        id++;
    }

    return NULL;
}

void* scheduler_thread(void* arg)
{
    PatientQueue* queue = (PatientQueue*)arg;

    while(runner) {
        pthread_mutex_lock(&queue_mutex);

        while(is_queue_empty(queue))
        {
            pthread_cond_wait(&queue_not_empty,
                              &queue_mutex);
        }

        PatientRecord p = dequeue_patient(queue);

        pthread_mutex_unlock(&queue_mutex);

        pthread_mutex_lock(&ward_mutex);

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