// Important! DO NOT MODIFY this file.
// You will not submit this file.
// You need to schedule specified workloads in this file.

#ifndef _WORKLOAD_HERADER_
#define _WORKLOAD_HERADER_

// Header file for virtual workloads
// DO NOT modify anything in this file if you couldn't guarantee.

#define NUM_TASKS 8

// Task IDs
enum {
	BUTTON = 0,
	TWOCOLOR,
	MOTION,
	ENCODER,
	SOUND,
	RGBCOLOR,
	ALED,
	BUZZER
};

#define thread_decl(NAME) \
extern int bExit_##NAME; \
void* thread_##NAME(void* param);

// Virtual workload + Body of sensing
thread_decl(button);
thread_decl(twocolor);
thread_decl(motion);
thread_decl(encoder);
thread_decl(sound);
thread_decl(rgbcolor);
thread_decl(aled);
thread_decl(buzzer);

// Virtual workload
void init_workload();
void finish_workload();

void workload_button();
void workload_track();
void workload_buzzer();
void workload_touch();
void workload_temp();
void workload_twocolor();
void workload_rgbcolor();
void workload_aled();

// Deadlines for 8 tasks (workload)
// Also see deadlines.c
extern long long workloadDeadlines[];

// You do not need to handle dependencies in this assignment.
// Ignore this declaration. 
extern int workloadDependencies[NUM_TASKS][NUM_TASKS];

#endif
