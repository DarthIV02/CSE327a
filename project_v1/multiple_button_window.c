#include <gtk/gtk.h>

static void activate(GtkApplication *app, void *user_data)
{
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget *grid   = gtk_grid_new();
  GtkWidget *label  = gtk_label_new("Hello World");

  //gtk_window_set_child(GTK_WINDOW(window), gtk_label_new("Hello World!"));

  gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);

  gtk_grid_set_column_spacing(GTK_GRID(grid),10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
  // Create a new button
  GtkWidget *button1 = gtk_button_new_with_label ("Press 1");
  GtkWidget *button2 = gtk_button_new_with_label ("Press 2");
  GtkWidget *button3 = gtk_button_new_with_label ("Press 3");

  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 3, 1);
  gtk_grid_attach(GTK_GRID(grid), button1, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button3, 2, 1, 1, 1);

  gtk_container_add(GTK_CONTAINER(window), grid);
  // When the button is clicked, close the window passed as an argument
  g_signal_connect_swapped (button1, "clicked", G_CALLBACK (gtk_window_close), 
  window);
  g_signal_connect_swapped (button2, "clicked", G_CALLBACK (gtk_window_close), 
  window);
  g_signal_connect_swapped (button3, "clicked", G_CALLBACK (gtk_window_close),
  window);

  gtk_widget_show_all(window);
}

int main(int argc, char *argv[])
{
  g_autoptr(GtkApplication) app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  return g_application_run(G_APPLICATION(app), argc, argv);
}