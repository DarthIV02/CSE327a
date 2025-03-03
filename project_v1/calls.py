import os
from twilio.rest import Client

def call_caretaker(client, caretaker_number):
    call = client.calls.create(
        twiml=f'<Response><Say>{HELP_TEXT}</Say></Response>',
        to=caretaker_number,
        from_=ORGINATE_NUMBER)
    print(call.sid)
    return call

def call_for_help():
    client = Client(ACCOUNT_SID, AUTH_TOKEN)
    call_caretaker(client, CARETAKER_NUMBER)

CARETAKER_NUMBER = os.getenv("CARETAKER_NUMBER", "") # your relative numbers comma-separated
print("Calling Phone Number: ", CARETAKER_NUMBER)
ORGINATE_NUMBER = os.getenv("TWILIO_NUMBER", "") # should be one of your Twilio active number
print("From number: ", ORGINATE_NUMBER)
PATIENT = os.getenv("PATIENT_NAME", "") # should be one of your Twilio active number
HELP_TEXT = f"Help call from {PATIENT} !"
print("Sending message: ", HELP_TEXT)
# Find your Account SID and Auth Token at twilio.com/console
# and set the environment variables. See http://twil.io/secure
ACCOUNT_SID = os.environ.get('TWILIO_ACCOUNT_SID', "")
AUTH_TOKEN = os.environ.get('TWILIO_AUTH_TOKEN', "")

call_for_help()