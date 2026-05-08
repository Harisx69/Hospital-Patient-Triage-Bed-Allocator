#ifndef THREADING_H
#define THREADING_H

#include <pthread.h>


extern pthread_mutex_t queue_mutex;
extern pthread_mutex_t ward_mutex;
extern pthread_cond_t queue_not_empty;
extern int runner;
void* receptionist_thread(void* arg);
void* scheduler_thread(void* arg);
void* discharge_listener_thread(void* arg);
void* monitor_thread(void* arg);

#endif