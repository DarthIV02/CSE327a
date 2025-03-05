#include "window.h"
#include "trigger_alarms.h"
#include "compartment.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <wiringPi.h>
#include <softPwm.h>

// Thread declaration macros
#define thread_decl(NAME) \
void* thread_##NAME(void* param) { \
	SharedVariable* pV = (SharedVariable*) param; \
	body_##NAME(pV); \
	return NULL; }

// Declare threads for each sensor/actuator function
thread_decl(motion)
thread_decl(container)

// Thread creation and joining macros
#define thread_create(NAME) pthread_create(&t_##NAME, NULL, thread_##NAME, &v);
#define thread_join(NAME) pthread_join(t_##NAME, NULL);

#define BUTTON 1

int main(int argc, char **argv) {
    
    // Initialize shared variable
	SharedVariable v;

	// Initialize WiringPi library
	if (wiringPiSetup() == -1) {
		printf("Failed to setup wiringPi.\n");
		return 1; 
	}

	// Initialize shared variable and sensors
	init_shared_variable(&v);
	init_sensors(&v);

    
    pinMode(BUTTON, INPUT);
    int val = 0;
    int window_opened = 0;

	// Thread identifiers
	pthread_t t_motion,
			  t_container;

    thread_create(motion);
    thread_create(container);

    // Create GTK application
    //pthread_t window_thread; //HEEEERE Window start
    //pthread_create(&window_thread, NULL, start_window, NULL); //HEEEERE Window start

    // Start the background task in a separate thread
    pthread_t countdown_alarm_thread;
    pthread_create(&countdown_alarm_thread, NULL, countdown_alarms, NULL);

    // Run concurrently
    //pthread_join(window_thread, NULL); //HEEEERE Window start
    pthread_join(countdown_alarm_thread, NULL);
    // Wait for all threads to finish
    thread_join(motion);
    thread_join(container);

    while(1){
        if (argc == 2){
            val = digitalRead(BUTTON);
            if (val == LOW && window_opened == 0){ // Only do it when the window is not running
                pthread_t window_thread;
                pthread_create(&window_thread, NULL, start_window, NULL);
                pthread_join(window_thread, NULL);
                window_opened = 1;
            }
        }

    }

    return 1;
}