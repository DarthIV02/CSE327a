#include "window.h"
#include "trigger_alarms.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    // Start the background task in a separate thread
    //pthread_t countdown_alarm_thread;
    //pthread_create(&countdown_alarm_thread, NULL, countdown_alarms, NULL);

    // Wait for the background thread to finish
    //pthread_join(countdown_alarm_thread, NULL);

    pthread_t window_thread;
    pthread_create(&window_thread, NULL, start_window, NULL);

    // Wait for the background thread to finish
    pthread_join(window_thread, NULL);

    // Create GTK application
    //int status = start_window(argc, argv);

    return 1;
}