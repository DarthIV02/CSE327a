#include "window.h"
#include "trigger_alarms.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    // Create GTK application
    int status = start_window(argc, argv);

    // Start the background task in a separate thread
    pthread_t countdown_alarm_thread;
    pthread_create(&countdown_alarm_thread, NULL, countdown_alarms, NULL);

    // Wait for the background thread to finish
    pthread_join(countdown_alarms, NULL);

    return status;
}