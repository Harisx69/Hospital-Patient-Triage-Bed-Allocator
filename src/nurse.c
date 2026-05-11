#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include "threading.h"

void* icu_nurse_thread(void* arg)
{
    (void)arg;

    while(runner)
    {
        sleep(1);

        if(!runner)
        {
            break;
        }

       if(rand() % 5 == 0)
{
    printf("ICU nurse checking beds\n");
}
    }

    return NULL;
}

void* general_nurse_thread(void* arg)
{
    (void)arg;

    while(runner)
    {
        sleep(1);

        if(!runner)
        {
            break;
        }
          if(rand() % 5 == 0)
        printf("General nurse checking beds\n");
    }

    return NULL;
}

void* isolation_nurse_thread(void* arg)
{
    (void)arg;

    while(runner) {
        sleep(1);

        if(!runner)
        {
            break;
        }
        if(rand() % 5 == 0)
        printf("Isolation nurse checking beds\n");
    }

    return NULL;
}