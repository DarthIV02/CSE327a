import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class MedicineSchedule(Gtk.Window):
    def __init__(self):
        super().__init__(title="Medicine Schedule")
        self.set_default_size(600, 400)

        # Main vertical layout
        self.vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.add(self.vbox)

        # Labels row at the top
        label_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=20)

        name_label = Gtk.Label(label="Medicine")
        name_label.set_xalign(0)  # Left-align the label
        label_box.pack_start(name_label, True, True, 0)

        time_label = Gtk.Label(label="Repeat every")
        time_label.set_xalign(0)
        label_box.pack_start(time_label, True, True, 0)

        start_label = Gtk.Label(label="Start Date")
        start_label.set_xalign(0)
        label_box.pack_start(start_label, True, True, 0)

        save_button = Gtk.Button(label="Save")
        save_button.connect("clicked", self.store_vals, label_box)
        label_box.pack_start(save_button, False, False, 0)

        self.vbox.pack_start(label_box, False, False, 5)

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
        self.add_medicine_entry()

        self.show_all()

    def add_medicine_entry(self, widget=None):
        # Horizontal box to hold medicine info
        medicine_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=10)

        # Medicine Name Entry
        name_entry = Gtk.Entry()
        name_entry.set_placeholder_text("Medicine Name")
        medicine_box.pack_start(name_entry, True, True, 0)

        # Time to take medicine
        time_entry = Gtk.Entry()
        time_entry.set_placeholder_text("Time (HH:MM)")
        medicine_box.pack_start(time_entry, True, True, 0)

        # Start date entry
        start_date_entry = Gtk.Entry()
        start_date_entry.set_placeholder_text("Start Date (YYYY-MM-DD)")
        medicine_box.pack_start(start_date_entry, True, True, 0)

        # Delete Button
        delete_button = Gtk.Button(label="❌")
        delete_button.connect("clicked", self.remove_medicine_entry, medicine_box)
        medicine_box.pack_start(delete_button, False, False, 0)

        # Add to the list and update UI
        self.medicine_list.pack_start(medicine_box, False, False, 0)
        self.show_all()

    def remove_medicine_entry(self, widget, medicine_box):
        # Remove the selected medicine block
        self.medicine_list.remove(medicine_box)
        self.show_all()

    def store_vals(self, widget, medicine_box):
        pass

# Run the application
win = MedicineSchedule()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()