#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<pthread.h>
#include<sys/wait.h>
#include"threading.h"
#include"queue.h"
#include"scheduler.h"
#include"memory_manager.h"
#include"ipc.h"
#include <pthread.h>
#include "threading.h"


void sigchld_handler(int sig)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void){
    int totalPatient = 3;
    int disChargeCount = 0;
    signal(SIGCHLD, sigchld_handler);

    initialize_ward();
    initialize_ipc();

    PatientQueue queue;
    init_queue(&queue);
    pthread_t receptionist;
    pthread_t scheduler;
    pthread_t discharge;
    pthread_t monitor;

pthread_mutex_init(&queue_mutex, NULL);
pthread_mutex_init(&ward_mutex, NULL);
pthread_cond_init(&queue_not_empty, NULL);

    PatientRecord p1 = {1, "Ali", 20, 8, 1, 1, time(NULL)};
    PatientRecord p2 = {2, "Ahmed", 22, 7, 2, 1, time(NULL)};
    PatientRecord p3 = {3, "Sara", 25, 6, 3, 1, time(NULL)};

    enqueue_patient(&queue, p1);
    enqueue_patient(&queue, p2);
    enqueue_patient(&queue, p3);

    pthread_create(&receptionist,
               NULL,
               receptionist_thread,
               &queue);

pthread_create(&scheduler,
               NULL,
               scheduler_thread,
               &queue);

pthread_create(&discharge,
               NULL,
               discharge_listener_thread,
               NULL);

pthread_create(&monitor,
               NULL,
               monitor_thread,
               &queue);

    int fd = open(DISCHARGE_FIFO, O_RDONLY);

    while(disChargeCount<totalPatient){
        int patient_id;

        int bytes = read(fd, &patient_id, sizeof(int));

        if(bytes > 0)
        {
            printf("Discharge received for Patient %d\n",
                   patient_id);

            free_partition(patient_id);
            print_ward_map();
            disChargeCount++;
        }
        
    }
    pthread_join(receptionist, NULL);
    pthread_join(scheduler, NULL);
    pthread_join(discharge, NULL);
    pthread_join(monitor, NULL);
    printf("Simulation Completed.\n");
    free_queue(&queue);
    cleanup_ipc();
    pthread_mutex_destroy(&queue_mutex);
    pthread_mutex_destroy(&ward_mutex);
    pthread_cond_destroy(&queue_not_empty);
    return 0;
}