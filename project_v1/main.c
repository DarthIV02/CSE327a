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
#include <pthread.h>

// Thread declaration macros
#define thread_decl(NAME) \
void* thread_##NAME(void* param) { \
	SharedVariable* pV = (SharedVariable*) param; \
	body_##NAME(pV); \
	return NULL; }

// Declare threads for each sensor/actuator function
//thread_decl(motion)
//thread_decl(container)

// Thread creation and joining macros
//#define thread_create(NAME) pthread_create(&t_##NAME, NULL, thread_##NAME, &v);
//#define thread_join(NAME) pthread_join(t_##NAME, NULL);

#define BUTTON 1

sem_t high_priority_sem;  // Define it only once here
sem_t low_priority_sem;
pthread_mutex_t lock;
int high_priority_waiting = 0;
int window_changed = 0;
int window_opened = 0;

int main(int argc, char **argv) {
    
    // Initialize shared variable
	SharedVariable v;

	// Initialize WiringPi library
	if (wiringPiSetup() == -1) {
		printf("Failed to setup wiringPi.\n");
		return 1; 
	}

	// Initialize shared variable and sensors
	//init_shared_variable(&v);
	//init_sensors(&v);

    // Initialize semaphores
    sem_init(&high_priority_sem, 0, 1);
    sem_init(&low_priority_sem, 0, 1);
    
    pinMode(BUTTON, INPUT);
    int val = 0;

	// Thread identifiers
	pthread_t t_motion,
			  t_container;

    //thread_create(motion); // MOVEMENT
    //thread_create(container);

    // Create GTK application
    pthread_t window_thread; //HEEEERE Window start
    //pthread_create(&window_thread, NULL, start_window, NULL); //HEEEERE Window start

    // Start the background task in a separate thread
    pthread_t countdown_alarm_thread;
    pthread_create(&countdown_alarm_thread, NULL, countdown_alarms, NULL);

    //thread_join(motion); // MOVEMENTTTTT
    //thread_join(container);

    struct tm last_dt;

    while(1){
        if (argc >= 2){
            val = digitalRead(BUTTON);
            if ((window_opened == 0 && val == LOW) || (window_opened == 0 && window_changed == 1)){ // Only do it when the window is not running
                pthread_create(&window_thread, NULL, start_window, NULL);
                window_opened = 1;
                sem_wait(&low_priority_sem);
                last_dt = get_time_from_hwclock();
                sem_post(&low_priority_sem);

            } else if(window_opened == 1 && window_changed == 0){
                sem_wait(&low_priority_sem);
                struct tm now_dt = get_time_from_hwclock();
                sem_post(&low_priority_sem);
                time_t current = mktime(&now_dt);
                time_t last = mktime(&last_dt);
                if (difftime(current, last) > 15){
                    printf("Delete thread?");
                    pthread_cancel(window_thread);
                }
                
            }
        }

    }    

    //pthread_join(window_thread, NULL);
    pthread_join(countdown_alarm_thread, NULL);

    return 1;
}