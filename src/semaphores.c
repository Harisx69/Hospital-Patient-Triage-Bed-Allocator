 #include <semaphore.h>
#include "semaphores.h"

sem_t icu_sem;
sem_t isolation_sem;
sem_t queue_full;
sem_t queue_empty;

void init_semaphores()
{
    sem_init(&icu_sem, 0, 4);
    sem_init(&isolation_sem, 0, 4);

    sem_init(&queue_full, 0, 0);
    sem_init(&queue_empty, 0, 20);
}

void destroy_semaphores()
{
    sem_destroy(&icu_sem);
    sem_destroy(&isolation_sem);

    sem_destroy(&queue_full);
    sem_destroy(&queue_empty);
}