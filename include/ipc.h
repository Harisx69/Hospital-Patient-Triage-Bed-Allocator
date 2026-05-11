#include "memory_manager.h"

#define DISCHARGE_FIFO "/tmp/discharge_fifo"
#define TRIAGE_FIFO "triage_fifo"
#define SHM_KEY 12345

void initialize_ipc();
void cleanup_ipc();
int create_shared_memory();
void remove_shared_memory(int shmid);