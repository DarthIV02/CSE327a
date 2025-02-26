import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class SimpleForm(Gtk.Window):
    def __init__(self):
        # Initialize the window
        super().__init__(title="Simple Form")
        self.set_default_size(400, 300)

        # Create a grid to organize the form elements
        grid = Gtk.Grid()
        self.add(grid)

        # Create the labels
        label1 = Gtk.Label(label="Name:")
        label2 = Gtk.Label(label="Age:")
        label3 = Gtk.Label(label="Notes:")

        # Create the text entry widgets for Name and Age
        self.name_entry = Gtk.Entry()
        self.age_entry = Gtk.Entry()

        # Create a large text box for the notes
        self.notes_textview = Gtk.TextView()
        self.notes_buffer = self.notes_textview.get_buffer()

        # Add the widgets to the grid
        grid.attach(label1, 0, 0, 1, 1)  # Label 1
        grid.attach(self.name_entry, 1, 0, 2, 1)  # Name input

        grid.attach(label2, 0, 1, 1, 1)  # Label 2
        grid.attach(self.age_entry, 1, 1, 2, 1)  # Age input

        grid.attach(label3, 0, 2, 1, 1)  # Label 3
        grid.attach(self.notes_textview, 1, 2, 2, 3)  # Notes text box

        # Add a submit button
        submit_button = Gtk.Button(label="Submit")
        submit_button.connect("clicked", self.on_submit_clicked)
        grid.attach(submit_button, 1, 5, 1, 1)  # Button below the form

    def on_submit_clicked(self, widget):
        # Get the values entered by the user
        name = self.name_entry.get_text()
        age = self.age_entry.get_text()

        start_iter, end_iter = self.notes_buffer.get_bounds()
        notes = self.notes_buffer.get_text(start_iter, end_iter, True)

        # Print the values (you can also do other actions like saving them)
        print(f"Name: {name}")
        print(f"Age: {age}")
        print(f"Notes: {notes}")

        # Optionally, you could show a confirmation dialog or clear the form
        dialog = Gtk.MessageDialog(self, 0, Gtk.MessageType.INFO, Gtk.ButtonsType.OK, "Form Submitted")
        dialog.format_secondary_text(f"Name: {name}\nAge: {age}\nNotes: {notes}")
        dialog.run()
        dialog.destroy()


# Create and run the form
win = SimpleForm()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()
