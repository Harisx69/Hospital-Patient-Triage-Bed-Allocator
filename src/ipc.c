#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ipc.h"





void initialize_ipc(){
    mkfifo(DISCHARGE_FIFO, 0666);
    mkfifo(TRIAGE_FIFO, 0666);   
}

void cleanup_ipc(){
    unlink(DISCHARGE_FIFO);
    unlink(TRIAGE_FIFO);    
}

int create_shared_memory(){
    int shmid;

    shmid = shmget(SHM_KEY,
                   sizeof(BedPartition) * WARD_SIZE,
                   IPC_CREAT | 0666);

    return shmid;
}
void remove_shared_memory(int shmid)
{
    shmctl(shmid, IPC_RMID, NULL);
}