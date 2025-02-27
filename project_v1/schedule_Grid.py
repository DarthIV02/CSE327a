import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
import json

class MedicineSchedule(Gtk.Window):
    def __init__(self):
        super().__init__(title="Medicine Schedule")
        self.set_default_size(800, 400)
        self.set_position(Gtk.WindowPosition.CENTER)
        self.medicine_array = []  # Store pointers for each medicine

        # Main vertical layout
        self.vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.add(self.vbox)

        # Grid for both Labels and Input Fields
        self.grid = Gtk.Grid()
        self.grid.set_column_spacing(10)
        self.grid.set_row_spacing(10)

        # Labels row (first row in the grid)
        self.grid.attach(Gtk.Label(label="Medicine"), 0, 0, 1, 1)
        self.grid.attach(Gtk.Label(label="Repeat every"), 1, 0, 1, 1)
        self.grid.attach(Gtk.Label(label="Start Date"), 2, 0, 1, 1)
        self.grid.attach(Gtk.Label(label="Last Hour Administered"), 3, 0, 1, 1)

        save_button = Gtk.Button(label="Save")
        save_button.connect("clicked", self.store_vals)
        self.grid.attach(save_button, 4, 0, 1, 1)

        self.vbox.pack_start(self.grid, False, False, 5)

        # Scrollable area
        self.scrolled_window = Gtk.ScrolledWindow()
        self.scrolled_window.set_policy(Gtk.PolicyType.AUTOMATIC, Gtk.PolicyType.AUTOMATIC)
        self.scrolled_window.set_min_content_height(300)

        # Medicine list container (Grid-based)
        self.scrolled_window.add(self.grid)

        self.vbox.pack_start(self.scrolled_window, True, True, 0)

        # Button to add a new medicine
        self.add_medicine_button = Gtk.Button(label="➕ Add Medicine")
        self.add_medicine_button.connect("clicked", self.add_medicine_entry)
        self.vbox.pack_start(self.add_medicine_button, False, False, 10)

        # Add first medicine block by default
        self.restore_vals()

        self.show_all()

    def add_medicine_entry(self, widget=None):
        row = len(self.medicine_array) + 1  # Get current row index (starting from row 1)

        # Medicine Name Entry
        name_entry = Gtk.Entry()
        name_entry.set_placeholder_text("Medicine Name")
        self.grid.attach(name_entry, 0, row, 1, 1)

        # Time to take medicine
        time_entry = Gtk.Entry()
        time_entry.set_placeholder_text("Time (HH:MM)")
        self.grid.attach(time_entry, 1, row, 1, 1)

        # Start date entry
        start_date_entry = Gtk.Entry()
        start_date_entry.set_placeholder_text("Start Date (YYYY-MM-DD)")
        self.grid.attach(start_date_entry, 2, row, 1, 1)

        last_time_entry = Gtk.Entry()
        last_time_entry.set_placeholder_text("Last time (HH:MM)")
        self.grid.attach(last_time_entry, 3, row, 1, 1)

        # Delete Button
        delete_button = Gtk.Button(label="❌")
        delete_button.connect("clicked", self.remove_medicine_entry, row)
        self.grid.attach(delete_button, 4, row, 1, 1)

        # Store reference
        self.medicine_array.append((name_entry, time_entry, start_date_entry, last_time_entry, delete_button))

        self.show_all()

    def remove_medicine_entry(self, widget, row):
        # Remove elements from grid
        for widget in self.medicine_array[row]:
            self.grid.remove(widget)

        del self.medicine_array[row]
        self.store_vals(saved_press=False)
        self.show_all()

    def store_vals(self, widget=None, saved_press=True):
        json_save = {}
        filename = "user_data/schedule.json"

        for i, (name_entry, time_entry, start_date_entry, last_time_entry, _) in enumerate(self.medicine_array):
            json_save[i] = {
                'name': name_entry.get_text(),
                'repeat': time_entry.get_text(),
                'start': start_date_entry.get_text(),
                'last_rec': last_time_entry.get_text()
            }

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

            for _ in range(len(data)):  # Add the previous boxes
                self.add_medicine_entry()

            # Restore text fields
            for i, (name_entry, time_entry, start_date_entry, last_time_entry, _) in enumerate(self.medicine_array):
                val = data[str(i)]
                name_entry.set_text(val['name'])
                time_entry.set_text(val['repeat'])
                start_date_entry.set_text(val['start'])
                last_time_entry.set_text(val['last_rec'])

        except:
            print("No saved data")
            self.add_medicine_entry()

# Run the application
win = MedicineSchedule()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()
