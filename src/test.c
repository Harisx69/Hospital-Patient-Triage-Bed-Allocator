#include <stdio.h>
#include <time.h>
#include "memory_manager.h"

int main()
{
    initialize_ward();

    PatientRecord p1 = {1, "Ali", 20, 8, 1, 1, time(NULL)};
    PatientRecord p2 = {2, "Ahmed", 25, 7, 2, 1, time(NULL)};
    PatientRecord p3 = {3, "Sara", 22, 5, 3, 1, time(NULL)};

    print_ward_map();

    best_fit_allocate(p1);
    best_fit_allocate(p2);
    best_fit_allocate(p3);

    print_ward_map();

    fragmentation_report();

    free_partition(2);

    print_ward_map();

    coalesce_memory();
    print_ward_map();

    fragmentation_report();

    paging_simulation(p1);

    return 0;
}