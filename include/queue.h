#ifndef QUEUE_H
#define QUEUE_H

#include "shared.h"

typedef struct QueueNode{
    PatientRecord patient;
    struct QueueNode* next;
} QueueNode;

typedef struct{
    QueueNode* front;
    QueueNode* rear;
} PatientQueue;

void init_queue(PatientQueue* queue);

QueueNode* create_node(PatientRecord patient);

void enqueue_patient(PatientQueue* queue, PatientRecord patient);

PatientRecord dequeue_patient(PatientQueue* queue);

int is_queue_empty(PatientQueue* queue);

void display_queue(PatientQueue* queue);

// void free_queue(PatientQueue* queue);

// int queue_size(PatientQueue* queue);

#endif