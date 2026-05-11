#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h>

#include "semaphores.h"
#include "threading.h"
#include "queue.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "ipc.h"
#include "statistics.h"
#include "nurse.h"


volatile sig_atomic_t runner = 1; 

void* triage_listener_thread(void* arg);
void sigchld_handler(int sig) {
    (void)sig;
    while(waitpid(-1, NULL, WNOHANG) > 0);
}


void handle_sigint(int sig) {
    (void)sig;
    runner = 0; 
    printf("\n[SIGNAL] SIGINT received. Preparing final report...\n");
}

int main(int argc, char* argv[]) { 
    char strategy[20] = "best";

    if(argc > 1) {
        strncpy(strategy, argv[1], 19);
    }

    printf("Hospital Ward Simulation\n");
    printf("Allocation Strategy: %s\n", strategy);
    printf("Press Ctrl+C to terminate and view the final report.\n\n");

    // Initialize signals
    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT, handle_sigint);

    // Initialize System Resources
    initialize_ward();
    initialize_ipc();
    init_semaphores();

    // Initialize Synchronization Primitives
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_mutex_init(&ward_mutex, NULL);
    pthread_cond_init(&queue_not_empty, NULL);

    // Initialize Patient Queue
    PatientQueue queue;
    init_queue(&queue);

    // Seed Initial Patients
    PatientRecord p1 = {1, "Ali", 20, 8, 1, 1, time(NULL)};
    PatientRecord p2 = {2, "Ahmed", 22, 7, 2, 1, time(NULL)};
    PatientRecord p3 = {3, "Sara", 25, 6, 3, 1, time(NULL)};

    pthread_mutex_lock(&queue_mutex);
    enqueue_patient(&queue, p1);
    enqueue_patient(&queue, p2);
    enqueue_patient(&queue, p3);
    pthread_cond_broadcast(&queue_not_empty);
    pthread_mutex_unlock(&queue_mutex);

    // Create Simulation Threads
    pthread_t receptionist, scheduler, discharge, monitor;
    pthread_t icu_nurse, general_nurse, isolation_nurse;

    // Change the thread variable name for clarity
    pthread_t triage_listener; 

    // Start the listener that waits for triage.sh
    pthread_create(&triage_listener, NULL, triage_listener_thread, &queue);
    pthread_create(&scheduler, NULL, scheduler_thread, &queue);
    pthread_create(&discharge, NULL, discharge_listener_thread, NULL);
    pthread_create(&monitor, NULL, monitor_thread, &queue);
    
    pthread_create(&icu_nurse, NULL, icu_nurse_thread, NULL);
    pthread_create(&general_nurse, NULL, general_nurse_thread, NULL);
    pthread_create(&isolation_nurse, NULL, isolation_nurse_thread, NULL);

    // Main thread enters wait state until Ctrl+C is pressed
    while(runner) {
        sleep(1);
    }

    // --- Shutdown and Reporting Sequence ---
    printf("\nShutting down threads...");
    
    // We cancel the infinite loops in the threads
    pthread_cancel(triage_listener);
    pthread_cancel(scheduler);
    pthread_cancel(discharge);
    pthread_cancel(monitor);
    pthread_cancel(icu_nurse);
    pthread_cancel(general_nurse);
    pthread_cancel(isolation_nurse);

    // Ensure all threads have stopped
    pthread_join(triage_listener, NULL);
    pthread_join(scheduler, NULL);
    pthread_join(discharge, NULL);
    pthread_join(monitor, NULL);

    printf("\nGenerating Final Statistics Report...");
    print_statistics(); //


    free_queue(&queue);
    cleanup_ipc();
    destroy_semaphores();
    pthread_mutex_destroy(&queue_mutex);
    pthread_mutex_destroy(&ward_mutex);
    pthread_cond_destroy(&queue_not_empty);

    printf("\nSimulation Completed Successfully.\n");

    return 0;
}