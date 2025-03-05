import asyncio
from bleak import BleakServer, BleakGATTCharacteristic, BleakGATTService

# Define a simple GATT service
SERVICE_UUID = "12345678-1234-5678-1234-56789abcdef0"
CHAR_UUID = "12345678-1234-5678-1234-56789abcdef1"

# Define a characteristic write callback
async def write_callback(sender, data):
    print(f"Received data from ESP32: {data.decode()}")

async def main():
    # Create a service
    service = BleakGATTService(SERVICE_UUID)
    characteristic = BleakGATTCharacteristic(
        CHAR_UUID,
        properties=["write", "read"],
        write_callback=write_callback,
    )
    
    # Add characteristic to service
    service.add_characteristic(characteristic)

    # Start the BLE server
    async with BleakServer([service]) as server:
        print("BLE Server Running. Waiting for ESP32...")
        await asyncio.Future()  # Keep the server running

asyncio.run(main())