#include <gtk/gtk.h>
#include <time.h>
#include "window.h"
#include "trigger_alarms.h"
#include "get_hw.h"
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>

// Global variables

int status;
gpointer alarm_pointer;
struct tm t;
struct tm t_last_update;
#define SEM_NAME "/sem_clock"
int clock_correct = 0;

static void update_time(gpointer user_data) { // Modify with real time clock ...
  GtkLabel *label = GTK_LABEL(user_data);
  t.tm_sec += 1;
  mktime(&t);
  
  char time_str[9]; // HH:MM:SS
  strftime(time_str, sizeof(time_str), "%H:%M:%S", &t);

  gtk_label_set_text(label, time_str);
  clock_correct = 1;
}

static void check_screen_change() { // Modify with real time clock ...
  time_t current = mktime(&t);
  time_t last = mktime(&t_last_update);

    // Debug
    /*printf("Last time med taken: %s\n", ctime(&last));
    printf("Difference: %lf\n", difftime(current, last));*/

    if (difftime(current, last) > 15){
      stop_window();
    }
}

// Function to apply CSS
static void apply_css(GtkWidget *widget) {
  GtkCssProvider *provider = gtk_css_provider_new();

  GError *error = NULL;

  if (!gtk_css_provider_load_from_path(provider, "./style.css", &error)) {
    g_warning("Failed to load CSS file: %s", error->message);
  } else {
    g_print("CSS file loaded successfully!\n");
  }

  GtkStyleContext *context = gtk_widget_get_style_context(widget);
  gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  g_object_unref(provider);
}

// Custom callback for button1
static void patient_clicked(GtkButton *button, gpointer user_data) {
  // You can add more actions here, such as closing the window or performing other tasks.
  // Correctly cast user_data to GtkWindow* before using it
  //GtkWindow *window = GTK_WINDOW(user_data);
  //gtk_window_close(window);  // Close the window
  // Your custom instructions
  system("./patient.sh");
}

// Custom callback for button2
static void schedule_clicked(GtkButton *button, gpointer user_data) {
  // Your custom instructions
  // Perform other tasks here
  // Correctly cast user_data to GtkWindow* before using it
  //GtkWindow *window = GTK_WINDOW(user_data);
  //gtk_window_close(window);  // Close the window
  system("./schedule.sh");
}

void change_alarm(int alarm_active){
  GtkWidget *alarm_here = GTK_WIDGET(alarm_pointer);
  if (alarm_active == 1){
    gtk_widget_set_name(alarm_here, "alarm-label");
    apply_css(alarm_here);
  } else {
    gtk_widget_set_name(alarm_here, "alarm-label-invisible");
    apply_css(alarm_here);
  }
  t_last_update = t;
}

static void activate (GtkApplication* app, gpointer user_data)
{
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget *grid   = gtk_grid_new();
  GtkWidget *label = gtk_label_new("00:00:00");
  GtkWidget *alarm = gtk_label_new("Time for medicine !");

  gtk_window_set_title (GTK_WINDOW (window), "Elder Care System");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Center the window
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  gtk_grid_set_column_spacing(GTK_GRID(grid),10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 6);

  // Allow grid to expand in both horizontal and vertical directions
  gtk_widget_set_hexpand(grid, TRUE);

  // Apply border class to grid
  //gtk_widget_set_name(grid, "grid");
  //apply_css(grid);

  // Add CSS class
  gtk_widget_set_name(label, "clock-label");
  apply_css(label);
  g_timeout_add_seconds(1, (GSourceFunc) update_time, label);

  //Configure style and visibility of alarm
  gtk_widget_set_name(alarm, "alarm-label-invisible");
  apply_css(alarm);
  //gtk_widget_hide(alarm);
  alarm_pointer = alarm;

  // Create buttons
  GtkWidget *patient_button = gtk_button_new_with_label ("Patient Details");
  GtkWidget *schedule_button = gtk_button_new_with_label ("Schedule");

  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), alarm, 0, 1, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), patient_button, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), schedule_button, 1, 2, 1, 1);

  // Allow widgets inside the grid to expand
  gtk_widget_set_hexpand(label, TRUE);
  gtk_widget_set_hexpand(alarm, TRUE);
  gtk_widget_set_hexpand(patient_button, TRUE);
  gtk_widget_set_hexpand(schedule_button, TRUE);

  // Create a container to center the grid
  gtk_widget_set_halign (grid, GTK_ALIGN_FILL);
  gtk_widget_set_valign (grid, GTK_ALIGN_CENTER);

  gtk_container_add(GTK_CONTAINER(window), grid);
  // When the button is clicked, close the window passed as an argument
  g_signal_connect(patient_button, "clicked", G_CALLBACK (patient_clicked), window);
  g_signal_connect(schedule_button, "clicked", G_CALLBACK (schedule_clicked), window);

  gtk_widget_show_all(window);
}

void* start_window ()
{
  /*sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
  if (sem == SEM_FAILED) {
      perror("sem_open");
      exit(EXIT_FAILURE);
  }*/
  pthread_mutex_lock(&lock);
  high_priority_waiting++; // Indicate that high-priority is waiting
  pthread_mutex_unlock(&lock);

  sem_wait(&high_priority_sem);
    
  t = get_time_from_hwclock();
  
  sem_post(&high_priority_sem); // Notify that data is ready

  pthread_mutex_lock(&lock);
  high_priority_waiting--; // Indicate that high-priority is done
  pthread_mutex_unlock(&lock);
  t_last_update = t;
  
  GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  
  // Run the application
  status = g_application_run(G_APPLICATION(app), NULL, NULL);
  
  // Clean up
  g_object_unref(app);
  
  return NULL;
}

void* stop_window ()
{
  GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);

  // Quit the application
  g_application_quit(G_APPLICATION(app));

  g_object_unref(app);

  return NULL;
}