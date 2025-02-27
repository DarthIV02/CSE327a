import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
import json

class MedicineSchedule(Gtk.Window):
    def __init__(self):
        super().__init__(title="Medicine Schedule")
        self.set_default_size(800, 400)
        self.set_position(Gtk.WindowPosition.CENTER)
        self.medicine_array = [] # Store pointers for each medicine

        # Main vertical layout
        self.vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.add(self.vbox)

        # Labels row at the top (Use a Grid for alignment)
        self.grid_labels = Gtk.Grid()
        self.grid_labels.set_column_spacing(10)
        self.grid_labels.set_row_spacing(5)

        self.grid_labels.attach(Gtk.Label(label="Medicine", xalign=0), 0, 0, 1, 1)
        self.grid_labels.attach(Gtk.Label(label="Repeat every", xalign=0), 1, 0, 1, 1)
        self.grid_labels.attach(Gtk.Label(label="Start Date", xalign=0), 2, 0, 1, 1)
        self.grid_labels.attach(Gtk.Label(label="Last Hour Administered", xalign=0), 3, 0, 1, 1)

        save_button = Gtk.Button(label="Save")
        save_button.connect("clicked", self.store_vals)
        self.grid_labels.attach(save_button, 4, 0, 1, 1)

        self.vbox.pack_start(self.grid_labels, False, False, 5)

        # Scrollable area
        self.scrolled_window = Gtk.ScrolledWindow()
        self.scrolled_window.set_policy(Gtk.PolicyType.AUTOMATIC, Gtk.PolicyType.AUTOMATIC)
        self.scrolled_window.set_min_content_height(300)

        # Medicine list container
        self.medicine_list = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.scrolled_window.add(self.medicine_list)

        self.vbox.pack_start(self.scrolled_window, True, True, 0)

        # Button to add a new medicine
        self.add_medicine_button = Gtk.Button(label="➕ Add Medicine")
        self.add_medicine_button.connect("clicked", self.add_medicine_entry)
        self.vbox.pack_start(self.add_medicine_button, False, False, 10)

        # Add first medicine block by default
        self.restore_vals()

        self.show_all()

    def add_medicine_entry(self, widget=None):
        # Use a Grid instead of a Box for structured layout
        medicine_grid = Gtk.Grid()
        medicine_grid.set_column_spacing(10)

        # Medicine Name Entry
        name_entry = Gtk.Entry()
        name_entry.set_placeholder_text("Medicine Name")
        medicine_grid.attach(name_entry, 0, 0, 1, 1)

        # Time to take medicine
        time_entry = Gtk.Entry()
        time_entry.set_placeholder_text("Time (HH:MM)")
        medicine_grid.attach(time_entry, 1, 0, 1, 1)

        # Start date entry
        start_date_entry = Gtk.Entry()
        start_date_entry.set_placeholder_text("Start Date (YYYY-MM-DD)")
        medicine_grid.attach(start_date_entry, 2, 0, 1, 1)

        # Last time administered
        last_time_entry = Gtk.Entry()
        last_time_entry.set_placeholder_text("Last time (HH:MM)")
        medicine_grid.attach(last_time_entry, 3, 0, 1, 1)

        # Delete Button
        delete_button = Gtk.Button(label="❌")
        delete_button.connect("clicked", self.remove_medicine_entry, medicine_grid)
        medicine_grid.attach(delete_button, 4, 0, 1, 1)

        # Add to the list and update UI
        self.medicine_list.pack_start(medicine_grid, False, False, 0)
        
        # Store pointer
        self.medicine_array.append(medicine_grid)

        self.show_all()

    def remove_medicine_entry(self, widget, medicine_box):
        # Remove the selected medicine block
        self.medicine_list.remove(medicine_box)
        self.medicine_array.remove(medicine_box)
        self.store_vals(saved_press=False)
        self.show_all()

    def store_vals(self, widget=None, label_box=None, saved_press = True):
        json_save = {}
        filename = "user_data/schedule.json"

        for i, box in enumerate(self.medicine_list):
            entry_values = []
            for child in box.get_children():
                if isinstance(child, Gtk.Entry):
                    entry_values.append(child.get_text())
            json_save[i] = {'name': entry_values[0], 'repeat': entry_values[1], 'start': entry_values[2], 'last_rec':entry_values[3]}

        with open(filename, "w") as file:
            json.dump(json_save, file)

        if saved_press:
            dialog = Gtk.MessageDialog(self, 0, Gtk.MessageType.INFO, Gtk.ButtonsType.OK, "Data Updated!")
            dialog.run()
            dialog.destroy()
    
    def restore_vals(self):
        try:
            with open('user_data/schedule.json') as f:
                data = json.load(f)
            
            for _ in range(len(data)): # Add the previous boxes
                self.add_medicine_entry()
            
            # Add the text
            for i, box in enumerate(self.medicine_array):
                val = ['name', 'repeat', 'start', 'last_rec']
                val_i = 0
                for child in box.get_children():
                    if isinstance(child, Gtk.Entry):
                        child.set_text(data[str(i)][val[val_i]])
                        val_i += 1
        except:  
            print("No saved data")
            self.add_medicine_entry()

# Run the application
win = MedicineSchedule()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()