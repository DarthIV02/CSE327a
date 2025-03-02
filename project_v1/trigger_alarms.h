#ifndef TRIGGER_H
#define TRIGGER_H

void* countdown_alarms();
int priority_clock;
struct tm get_time_from_hwclock();

#endif