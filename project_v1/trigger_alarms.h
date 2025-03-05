#ifndef TRIGGER_H
#define TRIGGER_H
#include <semaphore.h>  // Add this to fix the error

void* countdown_alarms();
extern sem_t high_priority_sem, low_priority_sem, wait_window_sem;
extern pthread_mutex_t lock;
extern int high_priority_waiting;
extern int window_changed;


#endif