#include <stdio.h>

int total_admitted = 0;
int total_discharged = 0;
float avg_waiting = 0;
float avg_turnaround = 0;
void print_statistics(){
   printf("\nReport generated.\n");
     
    printf("Total admitted: %d\n", total_admitted);

    printf("Total discharged: %d\n",total_discharged);
printf("Average waiting: %.2f\n",
       avg_waiting);

printf("Average turnaround: %.2f\n",
       avg_turnaround);
}