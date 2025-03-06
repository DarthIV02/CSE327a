#include "window.h"
#include "trigger_alarms.h"
#include "compartment.h"
#include "get_hw.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>
#include <time.h>

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
int efficient = 0;

int main(int argc, char **argv) {
    SharedVariable v;
    if (argc >= 2) {
        efficient = 1;
    }

    if (wiringPiSetup() == -1) {
        printf("Failed to setup wiringPi.\n");
        return 1;
    }

    sem_init(&high_priority_sem, 0, 1);
    sem_init(&low_priority_sem, 0, 1);
    
    pinMode(BUTTON, INPUT);
    int val = 0;

    pthread_t window_thread; // Thread for GTK window
    pthread_create(&window_thread, NULL, start_window, NULL);  // Create GTK window in a separate thread

    pthread_t countdown_alarm_thread;
    pthread_create(&countdown_alarm_thread, NULL, countdown_alarms, NULL);  // Assuming countdown_alarms exists

    struct tm last_dt;

    while (1) {
        if (efficient) {
            val = digitalRead(BUTTON);
            if (val == LOW || window_changed == 1) {
                // Ensure the window is opened only once
                pthread_create(&window_thread, NULL, start_window, NULL);
                pthread_join(window_thread, NULL);
            }
        }
    }

    pthread_join(countdown_alarm_thread, NULL);
    return 1;
}
