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
void learn_workloads(SharedVariable* v) {

	// TODO: Fill the body
	// This function is executed before the scheduling simulation.
	// You need to calculate the execution time of each thread here.

	//Could I set the rate here, to determine which of the 2 versions to run?

	void *(*functions[])(void *) = { thread_button, thread_sound, thread_encoder, thread_motion,
		thread_twocolor, thread_rgbcolor, thread_aled, thread_buzzer };

	int workloads[] = { BUTTON, SOUND, ENCODER, MOTION, TWOCOLOR, RGBCOLOR, ALED, BUZZER };

	long long time;

	for (int i = 0; i < NUM_TASKS; i++) {

		//High frequency

		set_by_max_freq();

		long long max_time = 0;

		for (int r = 0; r < 10; r++){
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

		for (int r = 0; r < 10; r++){
			time = get_current_time_us();
			functions[i](v);
			time = get_current_time_us() - time;
			if (time > max_time){
				max_time = time;
			}
		}

		//printDBG("Thread low %d has time %llu\n", workloads[i], max_time);
		v->workloadExecution_ind[workloads[i]+NUM_TASKS] = max_time;
    }

}

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

	/*for (int i = 0; i < NUM_TASKS; ++i) {
		if (aliveTasks[i] == 1) {
			prev_selection = i;
			break;
		}
	}*/

    // Get the sorted indices based on the array values
	if (sv->ordered == 0){
		getSortedIndices(workloadDeadlines, sv->deadlinesIndices); /*only run once*/
	}

	long long pred_time = 0;
	long long time;

	for (int i = 0; i < NUM_TASKS; ++i) {

		int act_idx = sv->deadlinesIndices[i];

		if (aliveTasks[act_idx] == 1) { // For each alive task
			if (prev_selection == -1){ 
				
				//Select the first task with earliest deadline
				prev_selection = act_idx;
			
			}
				
			//Check if you can run it at the slowest fequency
			time = get_scheduler_elapsed_time_us();
			long long closest_deadline = workloadDeadlines[act_idx];
			if((time % closest_deadline) + pred_time + sv->workloadExecution_ind[act_idx] > closest_deadline){ //Pass deadline
				prev_freq = 1; //Run it fast
				break;
			} else {
				//Doesnt break this deadline
				if (prev_selection == act_idx){
					pred_time += sv->workloadExecution_ind[act_idx + NUM_TASKS]; //Slowest it can run
				} else {
					pred_time += sv->workloadExecution_ind[act_idx]; //Fastest it can run
				}
			}

		}
	}

	// The retun value can be specified like this:
	TaskSelection sel;
	sel.task = prev_selection; // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)

	/*for (int i = 0; i < NUM_TASKS; i++) { //Print alive tasks
        printDBG("%d ", aliveTasks[i]);
    }
    printDBG("\n");*/

	/*for (int i = 0; i < NUM_TASKS; i++) { //Print deadline (they remain constant)
        printDBG("%llu ", workloadDeadlines[i]);
    }
    printDBG("\n");*/

	if (idleTime > 0){
		sv->total_idle_time += idleTime;
	}

	if (prev_freq == 0){

	}

	//printDBG("Freq %d \n", prev_freq);
	sel.freq = prev_freq; // Request the maximum frequency (if you want the minimum frequency, use 0 instead.)
	/*How to determine the best tasks to run at low frequency?*/

    return sel;
}
