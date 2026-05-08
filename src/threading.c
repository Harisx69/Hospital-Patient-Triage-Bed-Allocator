#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>

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

    while(1)
    {
        sleep(3);

        PatientRecord p;

        p.patient_id = id;
        p.priority = rand() % 5 + 1;
        p.care_units = 1;
        p.arrival_time = time(NULL);

        pthread_mutex_lock(&queue_mutex);

        enqueue_patient(queue, p);

        printf("Patient %d arrived\n", id);

        pthread_cond_signal(&queue_not_empty);

        pthread_mutex_unlock(&queue_mutex);

        id++;
    }

    return NULL;
}

void* scheduler_thread(void* arg)
{
    PatientQueue* queue = (PatientQueue*)arg;

    while(1)
    {
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

    while(1)
    {
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

    while(1)
    {
        sleep(5);

        pthread_mutex_lock(&ward_mutex);

        printf("\n===== MONITOR =====\n");

        print_ward_map();

        fragmentation_report();

        pthread_mutex_unlock(&ward_mutex);

        pthread_mutex_lock(&queue_mutex);

        printf("Queue Size: %d\n",
               queue_size(queue));

        pthread_mutex_unlock(&queue_mutex);
    }

    return NULL;
}