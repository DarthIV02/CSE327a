#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

// Note: Deadline of each workload is defined in the "workloadDeadlines" variable.
// i.e., You can access the dealine of the BUTTON thread using workloadDeadlines[BUTTON]

// Assignment: You need to implement the following two functions.

// learn_workloads(SharedVariable* v):
// This function is called at the start part of the program before actual scheduling
// - Parameters

// sv: The variable which is shared for every function over all threads

// Struct to hold values and their original indices
// Struct to hold the value and its original index
typedef struct {
    long long int value;
    int original_index;
} IndexedValue;

// Comparison function to sort based on value
int compare(const void *a, const void *b) {
    return ((IndexedValue*)a)->value - ((IndexedValue*)b)->value;
}

// Function to get the sorted indices based on the values
void getSortedIndices(long long int arr[], int sortedIndices[]) {
    // Create an array of IndexedValue structs
    IndexedValue indexedArr[NUM_TASKS];

    // Populate the indexed array
    for (int i = 0; i < NUM_TASKS; i++) {
        indexedArr[i].value = arr[i];
        indexedArr[i].original_index = i;
    }

    // Sort the indexed array based on the value field using qsort
    qsort(indexedArr, NUM_TASKS, sizeof(IndexedValue), compare);

    // Extract the sorted indices
    for (int i = 0; i < NUM_TASKS; i++) {
        sortedIndices[i] = indexedArr[i].original_index;
    }
}

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

	void *(*functions[])(void *) = { thread_button, thread_sound, thread_encoder, thread_motion,
		thread_twocolor, thread_rgbcolor, thread_aled, thread_buzzer };

	int workloads[] = { BUTTON, SOUND, ENCODER, MOTION, TWOCOLOR, RGBCOLOR, ALED, BUZZER };

	long long time;

	for (int i = 0; i < NUM_TASKS; i++) {

		//High frequency

		set_by_max_freq();

		long long max_time = 0;

		for (int r = 0; r < 5; r++){
			time = get_current_time_us();
			functions[i](v);
			time = get_current_time_us() - time;
			if (time > max_time){
				max_time = time;
			}
		}
		
		//printDBG("Thread high %d has time %llu\n", workloads[i], max_time);
		v->workloadExecution_ind[workloads[i]] = max_time;

		// Low frequency

		set_by_min_freq();
		
		max_time = 0;

		for (int r = 0; r < 5; r++){
			time = get_current_time_us();
			functions[i](v);
			time = get_current_time_us() - time;
			if (time > max_time){
				max_time = time;
			}
		}

		//printDBG("Thread low %d has time %llu\n", workloads[i], max_time);
		v->workloadExecution_ind[workloads[i]+8] = max_time;
    }

	getSortedIndices(workloadDeadlines, v->deadlinesIndices); /*only run once*/

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
	int prev_selection = -1;
	int prev_freq = 0;
	
	long long pred_time = 0;
	long long time = get_scheduler_elapsed_time_us();
	int act_idx;

	for (int i = 0; i < NUM_TASKS; i++) {

		act_idx = sv->deadlinesIndices[i];

		if (aliveTasks[act_idx] == 1) {
			if (prev_selection == -1){ 

				//Select the first task with earliest deadline
				prev_selection = act_idx;
			
			}
		}

		long long closest_deadline = workloadDeadlines[act_idx];
		if (prev_selection == act_idx){
			pred_time += sv->workloadExecution_ind[act_idx + 8]; //Slowest it can run
		} else {
			pred_time += sv->workloadExecution_ind[act_idx]; //Fastest it can run
		}
		if((time % closest_deadline) + pred_time > closest_deadline){ //Past and previous deadline
			prev_freq = 1; //Run it fast
			if (prev_selection != -1){
				break;
			}
		}
	}

	/*for (int i = 0; i < NUM_TASKS; ++i) {
		if (aliveTasks[i] == 1) {
			prev_selection = i;
			break;
		}
	}*/

	if (idleTime > 0){
		sv->total_idle_time += idleTime;
	}

	if (sv->prev_freq == 0){
		sv->total_low_time += ((time) - sv->prev_time);
	} else {
		sv->total_high_time += ((time) - sv->prev_time);
	}

	// The retun value can be specified like this:
	TaskSelection sel;
	sel.task = prev_selection; // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)
	sel.freq = prev_freq; // Request the maximum frequency (if you want the minimum frequency, use 0 instead.)
	sv->prev_time = time;
	//printDBG("Time: %llu\n", sv->prev_time);
	sv->prev_freq = prev_freq;

	//printDBG("Task %d ", sel.task);
	//printDBG("Freq %d\n", prev_freq);

    return sel;
}