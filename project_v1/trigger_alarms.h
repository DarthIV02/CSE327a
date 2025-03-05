#ifndef TRIGGER_H
#define TRIGGER_H

void* countdown_alarms();
struct tm get_time_from_hwclock();
sem_t high_priority_sem, low_priority_sem;
pthread_mutex_t lock;
int high_priority_waiting;


#endif