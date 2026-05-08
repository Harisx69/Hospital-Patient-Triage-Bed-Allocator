#include "queue.h"

void run_scheduler(PatientQueue* queue);
void write_schedule_log(char* message);
void fcfs_log(PatientRecord patient,
              int start,
              int end);