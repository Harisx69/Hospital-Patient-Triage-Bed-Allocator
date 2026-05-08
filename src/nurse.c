#include <stdio.h>
#include <unistd.h>
#include "threading.h"
#include "memory_manager.h"

void* icu_nurse_thread(void* arg)
{
    while(runner)
    {
        sleep(4);

        printf("ICU nurse checking beds\n");
    }

    return NULL;
}

void* general_nurse_thread(void* arg)
{
    while(runner)
    {
        sleep(5);

        printf("General nurse checking beds\n");
    }
}
    void* isolation_nurse_thread(void* arg){
    while(runner)
    {
        sleep(6);

        printf("Isolation nurse checking beds\n");
    }

    return NULL;
}

