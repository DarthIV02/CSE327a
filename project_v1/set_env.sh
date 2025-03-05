# setenv.sh
echo "Enter Twilio SID:"
read ssid
export TWILIO_ACCOUNT_SID="$ssid"
echo "Enter Twilio Authorization Token:"
read token
export TWILIO_AUTH_TOKEN="$token"