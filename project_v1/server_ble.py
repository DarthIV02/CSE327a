import dbus
import dbus.mainloop.glib
import dbus.service
from gi.repository import GLib
from bluez_peripheral.gatt import Characteristic, Service, Advertisement, Application
from bluez_peripheral.advert import LocalGATTManager

# Define the UUIDs
SERVICE_UUID = "12345678-1234-5678-1234-56789abcdef0"
CHAR_UUID = "12345678-1234-5678-1234-56789abcdef1"

class MyCharacteristic(Characteristic):
    def __init__(self, service):
        super().__init__(
            uuid=CHAR_UUID,
            flags=["read", "write"],
            service=service
        )
        self.value = b"Hello ESP32!"

    def ReadValue(self, options):
        print("ESP32 requested data")
        return self.value

    def WriteValue(self, value, options):
        print(f"Received from ESP32: {bytes(value).decode()}")
        self.value = bytes(value)

def main():
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    
    # Create GATT Service
    service = Service(SERVICE_UUID, primary=True)
    char = MyCharacteristic(service)
    service.add_characteristic(char)

    # Start Bluetooth Advertisement
    app = Application()
    app.add_service(service)
    
    manager = LocalGATTManager(app)
    manager.run()

    print("BLE Server Running...")
    GLib.MainLoop().run()

if __name__ == "__main__":
    main()