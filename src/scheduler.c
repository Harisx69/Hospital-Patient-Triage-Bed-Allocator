#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "scheduler.h"
#include "memory_manager.h"

void write_schedule_log(char* message)
{
    FILE* file = fopen("logs/schedule_log.txt", "a");

    if(file == NULL)
    {
        return;
    }

    fprintf(file, "%s\n", message);

    fclose(file);
}

void run_scheduler(PatientQueue* queue)
{
    while(!is_queue_empty(queue))
    {
        PatientRecord patient = dequeue_patient(queue);

        int bedIndex = best_fit_allocate(patient);

        if(bedIndex == -1)
        {
            printf("No bed available for Patient %d\n", patient.patient_id);
            continue;
        }

        pid_t pid = fork();

        if(pid == 0)
        {
            char patientIdStr[20];
            char bedStr[20];

            sprintf(patientIdStr, "%d", patient.patient_id);
            sprintf(bedStr, "%d", bedIndex);

            char* args[] = {
                "./patient_simulator",
                patientIdStr,
                bedStr,
                NULL
            };

            execv("./patient_simulator", args);

            exit(0);
            }

        char logBuffer[100];

        sprintf(logBuffer,
                "Patient %d assigned to bed %d",
                patient.patient_id,
                bedIndex);

        write_schedule_log(logBuffer);
    }
}
void fcfs_log(PatientRecord patient,
              int start,
              int end)
{
    FILE* file;

    file = fopen("logs/schedule_log.txt",
                 "a");

    if(file == NULL)
    {
        return;
    }

    fprintf(file,
            "P%d | Start: %d | End: %d\n",
            patient.patient_id,
            start,
            end);

    fclose(file);
}

