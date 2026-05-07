#include <stdio.h>
#include <string.h>
#include "memory_manager.h"

BedPartition ward[WARD_SIZE];
void initialize_ward()
{
    int i = 0 ;
    for(i = 0; i < WARD_SIZE; i++) {
        ward[i].partition_id = i;
        ward[i].start_unit = i;
        ward[i].size = 1;
        ward[i].is_free = 1;
        ward[i].patient_id = -1;
        if(i < 4) {
            strcpy(ward[i].bed_type, "ICU");
        }else if(i < 8){
            strcpy(ward[i].bed_type, "ISOLATION");
        }else{
            strcpy(ward[i].bed_type, "GENERAL");
        }
    }
}

void print_ward_map(){
    int i;
     printf("\nWard Status:\n");
    for(i = 0; i < WARD_SIZE; i++) {
        if (i==WARD_SIZE/2) printf("\n");
        if(ward[i].is_free) {
            printf("[FREE]");
        }else{
            printf("[P%d]", ward[i].patient_id);
        }
    }
     printf("\n");
}
int best_fit_allocate(PatientRecord patient){
    int bestIndex = -1;
    int smallestGap = 9999;
    int i;

    for(i = 0; i < WARD_SIZE; i++){
        if(ward[i].is_free){
            int gap = ward[i].size - patient.care_units;

            if(gap >= 0 && gap < smallestGap){
                smallestGap = gap;
                bestIndex = i;
            }
        }
    }

    if(bestIndex != -1) {
        ward[bestIndex].is_free = 0;
        ward[bestIndex].patient_id = patient.patient_id;

        return bestIndex;
    }
     return -1;
}
int first_fit_allocate(PatientRecord patient){
    int i = 0;

    for(i = 0; i < WARD_SIZE; i++){
        if(ward[i].is_free)
        {
            ward[i].is_free = 0;
            ward[i].patient_id = patient.patient_id;

            return i;
        }
    }

    return -1;
}

int worst_fit_allocate(PatientRecord patient)
{
    int worstIndex = -1;
    int largestGap = -1;
    int i;

    for(i = 0; i < WARD_SIZE; i++)
    {
        if(ward[i].is_free)
        {
            int gap = ward[i].size - patient.care_units;

            if(gap >= 0 && gap > largestGap)
            {
                largestGap = gap;
                worstIndex = i;
            }
        }
    }

    if(worstIndex != -1)
    {
        ward[worstIndex].is_free = 0;
        ward[worstIndex].patient_id = patient.patient_id;
return worstIndex;
    }

    return -1;
}
void free_partition(int patient_id){
    int i;
    for(i = 0; i < WARD_SIZE; i++)  {
        if(ward[i].patient_id == patient_id) {
            ward[i].is_free = 1;
            ward[i].patient_id = -1;
        }
    }
}
void coalesce_memory()
{
    int i;
    for(i = 0; i < WARD_SIZE - 1; i++)
    {
        if(ward[i].is_free && ward[i + 1].is_free)
        {
            ward[i].size += ward[i + 1].size;
            ward[i + 1].size = 0;
        }
    }
}
int calculate_total_free_units()
{
    int total = 0;
    int i;

    for(i = 0; i < WARD_SIZE; i++)
    {
        if(ward[i].is_free)
        {
            total += ward[i].size;
        }
    }

    return total;
}
int largest_free_block()
{
    int largest = 0;
    int i;

    for(i = 0; i < WARD_SIZE; i++)
    {
        if(ward[i].is_free)
        {
            if(ward[i].size > largest)
            {
                largest = ward[i].size;
            }
        }
    }

    return largest;
}
void fragmentation_report()
{
    int totalFree = calculate_total_free_units();
    int largestBlock = largest_free_block();

    float fragmentation = 0;

    if(totalFree > 0)
    {
        fragmentation = (1.0 - ((float)largestBlock / totalFree)) * 100;
    }

    printf("\nFragmentation Report\n");
    printf("Total Free Units: %d\n", totalFree);
    printf("Largest Free Block: %d\n", largestBlock);
    printf("External Fragmentation: %.2f%%\n", fragmentation);
}
void paging_simulation(PatientRecord patient)
{
    int pageSize = 2;

    int pagesNeeded = patient.care_units / pageSize;

    if(patient.care_units % pageSize != 0)
    {
        pagesNeeded++;
    }

    int allocatedUnits = pagesNeeded * pageSize;

    int internalFragmentation = allocatedUnits - patient.care_units;

    printf("\nPaging Simulation\n");
    printf("Pages Needed: %d\n", pagesNeeded);
    printf("Internal Fragmentation: %d\n", internalFragmentation);
}