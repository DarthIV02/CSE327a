#ifndef TRIGGER_H
#define TRIGGER_H
#include <semaphore.h>  // Add this to fix the error

void* countdown_alarms();
struct tm get_time_from_hwclock();
extern sem_t high_priority_sem, low_priority_sem;
extern pthread_mutex_t lock;
extern int high_priority_waiting;


#endif