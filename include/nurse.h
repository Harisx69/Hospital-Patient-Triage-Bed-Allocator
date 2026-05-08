#ifndef NURSE_H
#define NURSE_H

void* icu_nurse_thread(void* arg);
void* general_nurse_thread(void* arg);
void* isolation_nurse_thread(void* arg);

#endif