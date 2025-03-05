#!/usr/bin/python3

import sys
import asyncio
from bleak import BleakClient
from calls import *

DEVICE = ""
CHARACTERISTIC = "19b10001-e8f2-537e-4f6c-d104768a1214"
called = 0
ACCOUNT_SID = None
AUTH_TOKEN = None

async def main():
    async with BleakClient(DEVICE) as client:
        dev_value = await client.read_gatt_char(CHARACTERISTIC)
        input = int(dev_value[0])
        print("Device Value: {0}".format(input))
        if input == 1 and called == 0:
            CARETAKER_NUMBER, ORGINATE_NUMBER, PATIENT, HELP_TEXT = get_info()
            call_for_help(ACCOUNT_SID, AUTH_TOKEN, CARETAKER_NUMBER, HELP_TEXT, ORGINATE_NUMBER)
            called = 1
        if input == 0 and called == 1:
            called = 0

if len(sys.argv) < 2:
    print("Usage: {} <addr>".format(sys.argv[0]))
    sys.exit(1)

DEVICE = sys.argv[1]

while True:
    asyncio.run(main())