#ifndef SHARED_H
#define SHARED_H

#include <time.h>

typedef struct
{
    int patient_id;
    char name[64];
    int age;
    int severity;
    int priority;
    int care_units;
    time_t arrival_time;
} PatientRecord;

typedef struct
{
    int partition_id;
    int start_unit;
    int size;
    int is_free;
    int patient_id;
    time_t arrival_time;
    char bed_type[16];
} BedPartition;

#endif