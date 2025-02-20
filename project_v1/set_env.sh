# setenv.sh
echo "Enter Twilio SID:"
read ssid
export TWILIO_ACCOUNT_SID="$ssid"
echo "Enter Twilio Authorization Token:"
read token
export TWILIO_AUTH_TOKEN="$token"
echo "Enter Twilio Number:"
read number_1
export TWILIO_NUMBER="$number_1"
echo "Enter Patient Name:"
read name
export PATIENT_NAME="$name"
echo "Enter Caretaker Phone Number:"
read number_2
export CARETAKER_NUMBER="$number_2"