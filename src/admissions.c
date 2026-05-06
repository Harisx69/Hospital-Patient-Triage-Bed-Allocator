#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include "queue.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "ipc.h"

void sigchld_handler(int sig)
{
    while(waitpid(-1, NULL, WNOHANG) > 0)
    {
    }
}

int main()
{
    signal(SIGCHLD, sigchld_handler);

    initialize_ward();
    initialize_ipc();

    PatientQueue queue;
    init_queue(&queue);

    PatientRecord p1 = {1, "Ali", 20, 8, 1, 1, time(NULL)};
    PatientRecord p2 = {2, "Ahmed", 22, 7, 2, 1, time(NULL)};
    PatientRecord p3 = {3, "Sara", 25, 6, 3, 1, time(NULL)};

    enqueue_patient(&queue, p1);
    enqueue_patient(&queue, p2);
    enqueue_patient(&queue, p3);

    run_scheduler(&queue);

    int fd = open(DISCHARGE_FIFO, O_RDONLY);

    while(1)
    {
        int patient_id;

        int bytes = read(fd, &patient_id, sizeof(int));

        if(bytes > 0)
        {
            printf("Discharge received for Patient %d\n",
                   patient_id);

            free_partition(patient_id);
            print_ward_map();
        }
    }

    cleanup_ipc();

    return 0;
}