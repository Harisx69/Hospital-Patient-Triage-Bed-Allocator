#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "shared.h"

#define WARD_SIZE 20

extern BedPartition ward[WARD_SIZE];

void initialize_ward();
int best_fit_allocate(PatientRecord patient);
int first_fit_allocate(PatientRecord patient);
int worst_fit_allocate(PatientRecord patient);
void free_partition(int patient_id);
void coalesce_memory();
void print_ward_map();
void fragmentation_report();
int calculate_total_free_units();
int largest_free_block();
void paging_simulation(PatientRecord patient);

#endif