#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include"queue.h"
#include"shared.h"


void init_queue(PatientQueue* queue){
    queue->front = NULL;
    queue->rear = NULL;
}

QueueNode* create_node(PatientRecord patient){
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));

    if(newNode == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }

    newNode->patient = patient;
    newNode->next = NULL;

    return newNode;
}
void enqueue_patient(PatientQueue* queue,
                     PatientRecord patient)
{
    QueueNode* node;

    node = create_node(patient);

    if(queue->front == NULL)
    {
        queue->front = node;
        queue->rear = node;

        return;
    }

    if(patient.priority <
       queue->front->patient.priority)
    {
        node->next = queue->front;

        queue->front = node;

        return;
    }

    QueueNode* current;

    current = queue->front;

    while(current->next != NULL &&
          current->next->patient.priority
          < patient.priority)
    {
        current = current->next;
    }

    node->next = current->next;

    current->next = node;

    if(node->next == NULL)
    {
        queue->rear = node;
    }
}
PatientRecord dequeue_patient(PatientQueue* queue){
    PatientRecord emptyPatient;

    QueueNode* temp = queue->front;

    if(queue->front == NULL) {
        emptyPatient.patient_id = -1;
        return emptyPatient;
    }

    PatientRecord removedPatient = temp->patient;

    queue->front = queue->front->next;

    if(queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);

    return removedPatient;
}
int is_queue_empty(PatientQueue* queue){
    return queue->front == NULL;
}
void display_queue(PatientQueue* queue)
{
    QueueNode* current;

    current = queue->front;

    printf("\nQueue:\n");

    while(current != NULL)
    {
        printf("[P%d | Pr:%d] -> ",
               current->patient.patient_id,
               current->patient.priority);

        current = current->next;
    }

    printf("NULL\n");
}
void free_queue(PatientQueue* queue){
    QueueNode* current = queue->front;

    while(current != NULL){
        QueueNode* temp = current;
        current = current->next;
        free(temp);
    }

    queue->front = NULL;
    queue->rear = NULL;
}

int queue_size(PatientQueue* queue){
    int size = 0;
    QueueNode* current = queue->front;

    while(current != NULL) {   size++; current = current->next;
    }

    return size;
}