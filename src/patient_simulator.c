#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "ipc.h"

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        return 1;
    }

    int patient_id = atoi(argv[1]);
    int bed_id = atoi(argv[2]);

    srand(time(NULL) + patient_id);

    int treatment_time = rand() % 8 + 2;

    printf("Patient %d admitted to bed %d\n",
           patient_id,
           bed_id);
               printf("Patient %d receiving treatment\n",
           patient_id);

    sleep(treatment_time);

    printf("Patient %d discharged\n",
           patient_id);

    int fd = open(DISCHARGE_FIFO, O_WRONLY);

    if(fd != -1)
    {
        write(fd, &patient_id, sizeof(int));
        close(fd);
    }

    return 0;
}