#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"

#include <stdio.h>
#include <limits.h>


// Note: Deadline of each workload is defined in the "workloadDeadlines" variable.
// i.e., You can access the dealine of the BUTTON thread using workloadDeadlines[BUTTON]

// Assignment: You need to implement the following two functions.

// learn_workloads(SharedVariable* v):
// This function is called at the start part of the program before actual scheduling
// - Parameters

// sv: The variable which is shared for every function over all threads
void learn_workloads(SharedVariable* v) {
	// TODO: Fill the body
	// This function is executed before the scheduling simulation.
	// You need to calculate the execution time of each thread here.
	//
	// Thread functions: 
	// thread_button, thread_touch, thread_encoder, thread_tilt,
	// thread_twocolor, thread_rgbcolor, thread_aled, thread_buzzer

	// Tip 1. You can call each workload function here like:
	// thread_button();

	// Tip 2. You can get the current time here like:
	// long long curTime = get_current_time_us();
}


// select_task(SharedVariable* sv, const int* aliveTasks):
// This function is called while runnning the actual scheduler
// - Parameters
// sv: The variable which is shared for every function over all threads
// aliveTasks: an array where each element indicates whether the corresponed task is alive(1) or not(0).
// idleTime: a time duration in microsecond. You can know how much time was waiting without any workload
//           (i.e., it's larger than 0 only when all threads are finished and not reach the next preiod.)
// - Return value
// TaskSelection structure which indicates the scheduled task and the CPU frequency
TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {
	// TODO: Fill the body
	// This function is executed inside of the scheduling simulation.
    // You need to implement a EDF scheduler.

    // Goal: Select a task that has the earliest deadline among the ready tasks.

	// Tip 1. You may get the current time elapsed in the scheduler here like:
	// long long curTime = get_scheduler_elapsed_time_us();

	// Tip 2. The readiness and completeness of tasks are indicated by
	// the flip of aliveTasks.
	// For example, when aliveTasks[i] switches from 1 (alive) to 0 
	// (not alive), it indicates that task i is finished for the current
	// round.
	// When aliveTasks[i] switches from 0 to 1, it means task i is ready
	// to schedule for its next period.

	// DO NOT make any interruptable / IO tasks in this function.
	// You can use printDBG instead of printf.

	// Starter scheduler: Round robin
	// It selects a next thread using aliveTasks.
	static int prev_selection = -1;

	for (int i = 0; i < NUM_TASKS; ++i) {
		if (aliveTasks[i] == 1) {
			prev_selection = i;
			break;
		}
	}

	// The retun value can be specified like this:
	TaskSelection sel;
	sel.task = prev_selection; // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)
	sel.freq = 1; // Request the maximum frequency (if you want the minimum frequency, use 0 instead.)

    return sel;
}
