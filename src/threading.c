#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "statistics.h"
#include "threading.h"
#include "queue.h"
#include "memory_manager.h"
#include "ipc.h"


extern volatile sig_atomic_t runner; 
extern int total_admitted;
extern int total_discharged;
extern float avg_waiting;
extern float avg_turnaround;

pthread_mutex_t queue_mutex;
pthread_mutex_t ward_mutex;
pthread_cond_t queue_not_empty;


void* receptionist_thread(void* arg){
    PatientQueue* queue = (PatientQueue*)arg;
    int id = 10;

    while(runner) 
    {
        sleep(3);
        if(!runner) break;

        PatientRecord p;
        p.patient_id = id;
        p.priority = rand() % 5 + 1;
        p.care_units = 1;
        p.arrival_time = time(NULL);

        pthread_mutex_lock(&queue_mutex);
        enqueue_patient(queue, p);
        printf("Patient %d arrived (Auto-generated)\n", id);
        pthread_cond_signal(&queue_not_empty);
        pthread_mutex_unlock(&queue_mutex);

        id++;
    }
    return NULL;
}


void* scheduler_thread(void* arg)
{
    PatientQueue* queue = (PatientQueue*)arg;

    while(runner)
    {
        pthread_mutex_lock(&queue_mutex);


        while(runner && is_queue_empty(queue))
        {
            pthread_cond_wait(&queue_not_empty, &queue_mutex);
        }


        if(!runner) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }

        PatientRecord p = dequeue_patient(queue);
        pthread_mutex_unlock(&queue_mutex);

        pthread_mutex_lock(&ward_mutex);
        int bed = best_fit_allocate(p);

        if(bed != -1){
            total_admitted++;
            long wait_time = time(NULL) - p.arrival_time;
            

            avg_waiting = ((avg_waiting * (total_admitted - 1)) + wait_time) / total_admitted;
            

            ward[bed].arrival_time = p.arrival_time; 
        }
        pthread_mutex_unlock(&ward_mutex);

        if(bed == -1)
        {
            printf("No bed for patient %d\n", p.patient_id);
            continue;
        }

        pid_t pid = fork();
        if(pid == 0)
        {
            char id_str[20], bed_str[20];
            sprintf(id_str, "%d", p.patient_id);
            sprintf(bed_str, "%d", bed);

            char* args[] = {"./patient_simulator", id_str, bed_str, NULL};
            execv("./patient_simulator", args);
            perror("execv failed");
            exit(1); 
        }
    }
    return NULL;
}


void* discharge_listener_thread(void* arg)
{

    int fd = open(DISCHARGE_FIFO, O_RDONLY | O_NONBLOCK);
    if(fd == -1) return NULL;

    while(runner)
    {
        int patient_id;
        // Use read() on the non-blocking file descriptor
        int bytes = read(fd, &patient_id, sizeof(int));

        if(bytes > 0)
        {
            pthread_mutex_lock(&ward_mutex);

            time_t arrival_time = 0;

            for(int i = 0; i < WARD_SIZE; i++) {
                if(!ward[i].is_free && ward[i].patient_id == patient_id) {
                    arrival_time = ward[i].arrival_time; 
                    break;
                }
            }

            free_partition(patient_id); 
            total_discharged++;

            if(arrival_time > 0) {
                // Formula: $$T_{turnaround} = T_{discharge} - T_{arrival}$$
                long turnaround_time = time(NULL) - arrival_time;
                avg_turnaround = ((avg_turnaround * (total_discharged - 1)) + turnaround_time) / total_discharged;
            }

            pthread_mutex_unlock(&ward_mutex);
            printf("Patient %d discharged | Turnaround: %ld sec\n", patient_id, (time(NULL) - arrival_time));
        }
        else {
            usleep(100000); 
        }
    }
    close(fd);
    return NULL;
}


void* monitor_thread(void* arg)
{
    PatientQueue* queue = (PatientQueue*)arg;
    while(runner)
    {
        sleep(5);
        if(!runner) break;

        pthread_mutex_lock(&ward_mutex);
        printf("\n===== MONITOR =====\n");
        print_ward_map();
        fragmentation_report();
        pthread_mutex_unlock(&ward_mutex);

        pthread_mutex_lock(&queue_mutex);
        printf("Queue Size: %d\n", queue_size(queue));
        pthread_mutex_unlock(&queue_mutex);
    }
    return NULL;
}

void* triage_listener_thread(void* arg) {
    PatientQueue* queue = (PatientQueue*)arg;

    while(runner) {

        int fd = open(TRIAGE_FIFO, O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            usleep(500000);
            continue;
        }


        fcntl(fd, F_SETFL, 0);
        FILE* fifo = fdopen(fd, "r");

        int id, priority, units;
        while (runner && fscanf(fifo, "%d %d %d", &id, &priority, &units) != EOF) {
            PatientRecord p;
            p.patient_id = id;
            p.priority = priority;
            p.care_units = units;
            p.arrival_time = time(NULL); 

            pthread_mutex_lock(&queue_mutex);
            enqueue_patient(queue, p);
            printf("\n[TRIAGE] Patient %d received from script (Priority: %d)\n", id, priority);
            pthread_cond_signal(&queue_not_empty); 
            pthread_mutex_unlock(&queue_mutex);
        }
        fclose(fifo);
    }
    return NULL;
}