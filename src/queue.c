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
void enqueue_patient(PatientQueue* queue, PatientRecord patient)
{
    QueueNode* newNode = create_node(patient);

    if(newNode == NULL) {
        return;
    }

    if(queue->front == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
        return;
    }

    if(patient.priority < queue->front->patient.priority){
        newNode->next = queue->front;
        queue->front = newNode;
        return;
    }

    QueueNode* current = queue->front;

    while(current->next != NULL &&
          current->next->patient.priority <= patient.priority){
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    if(newNode->next == NULL) {
        queue->rear = newNode;
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
void display_queue(PatientQueue* queue){
    QueueNode* current = queue->front;

    while(current != NULL){
        printf("Patient ID: %d Priority: %d\n",
               current->patient.patient_id,
               current->patient.priority);

        current = current->next;
    }
}