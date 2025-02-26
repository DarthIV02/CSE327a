#include <gtk/gtk.h>
#include <time.h>

static void update_time(GtkLabel *label) { // Modify with real time clock ...
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  char time_str[9]; // HH:MM:SS
  strftime(time_str, sizeof(time_str), "%H:%M:%S", t);

  gtk_label_set_text(label, time_str);
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

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget *grid   = gtk_grid_new();
  GtkWidget *label = gtk_label_new("00:00:00");

  gtk_window_set_title (GTK_WINDOW (window), "Elder Care System");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Center the window
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

  gtk_grid_set_column_spacing(GTK_GRID(grid),10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 6);

  // Add CSS class
  gtk_widget_set_name(label, "clock-label");
  apply_css(label);

  // Create buttons
  GtkWidget *button1 = gtk_button_new_with_label ("Patient Details");
  GtkWidget *button2 = gtk_button_new_with_label ("Schedule");

  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 3, 1);
  gtk_grid_attach(GTK_GRID(grid), button1, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 1, 1, 1, 1);

  gtk_container_add(GTK_CONTAINER(window), grid);
  // When the button is clicked, close the window passed as an argument
  g_signal_connect_swapped (button1, "clicked", G_CALLBACK (gtk_window_close), 
  window);
  g_signal_connect_swapped (button2, "clicked", G_CALLBACK (gtk_window_close), 
  window);

  gtk_widget_show_all(window);
}

int
main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}