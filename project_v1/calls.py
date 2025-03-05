import os
from twilio.rest import Client
import json

def call_caretaker(client, caretaker_number, HELP_TEXT, ORGINATE_NUMBER):
    call = client.calls.create(
        twiml=f'<Response><Say>{HELP_TEXT}</Say></Response>',
        to=caretaker_number,
        from_=ORGINATE_NUMBER)
    print(call.sid)
    return call

def call_for_help(ACCOUNT_SID, AUTH_TOKEN, CARETAKER_NUMBER, HELP_TEXT, ORGINATE_NUMBER):
    client = Client(ACCOUNT_SID, AUTH_TOKEN)
    call_caretaker(client, CARETAKER_NUMBER, HELP_TEXT, ORGINATE_NUMBER)

def get_info():

    # Load stored info
    try:
        with open('user_data/patient.json') as f:
            data = json.load(f)
    except:
        data = None


    CARETAKER_NUMBER = '+16199538664' # your relative numbers comma-separated
    print("Calling Phone Number: ", CARETAKER_NUMBER)
    ORGINATE_NUMBER = '+18055907861' # should be one of your Twilio active number
    print("From number: ", ORGINATE_NUMBER)
    PATIENT = data["name"]
    HELP_TEXT = f"Help call from {PATIENT}. I repeat this is a call from patient {PATIENT}, the help button has been pushed"
    print("Sending message: ", HELP_TEXT)
    # Find your Account SID and Auth Token at twilio.com/console
    # and set the environment variables. See http://twil.io/secure
    #ACCOUNT_SID = os.environ.get('TWILIO_ACCOUNT_SID', "")
    #print("ACCOUNT_SID: ", ACCOUNT_SID)
    #AUTH_TOKEN = os.environ.get('TWILIO_AUTH_TOKEN', "")

    return CARETAKER_NUMBER, ORGINATE_NUMBER, PATIENT, HELP_TEXT#, ACCOUNT_SID, AUTH_TOKEN