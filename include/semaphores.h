#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <semaphore.h>

extern sem_t icu_sem;
extern sem_t isolation_sem;
extern sem_t queue_full;
extern sem_t queue_empty;

void init_semaphores();
void destroy_semaphores();

#endif