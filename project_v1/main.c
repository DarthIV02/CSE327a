#include "window.h"
#include "trigger_alarms.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    
    // Create GTK application
    pthread_t window_thread;
    pthread_create(&window_thread, NULL, start_window, NULL);

    // Start the background task in a separate thread
    pthread_t countdown_alarm_thread;
    pthread_create(&countdown_alarm_thread, NULL, countdown_alarms, NULL);

    // Run concurrently
    pthread_join(window_thread, NULL);
    pthread_join(countdown_alarm_thread, NULL);

    return 1;
}