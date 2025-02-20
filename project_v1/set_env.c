#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char twilio_SSID[256];
    char twilio_token[256];
    char patient[256];
    char caretaker_number[256];
    char twilio_number[256];

    // Ask the user for input
    printf("Enter Twilio SID: ");
    fgets(twilio_SSID, sizeof(twilio_SSID), stdin);
    twilio_SSID[strcspn(twilio_SSID, "\n")] = '\0';  // Remove newline character

    printf("Enter Twilio Token: ");
    fgets(twilio_token, sizeof(twilio_token), stdin);
    twilio_token[strcspn(twilio_token, "\n")] = '\0';  // Remove newline character

    printf("Enter Patient Name: ");
    fgets(patient, sizeof(patient), stdin);
    patient[strcspn(patient, "\n")] = '\0';  // Remove newline character

    printf("Enter Caretaker Number: ");
    fgets(caretaker_number, sizeof(caretaker_number), stdin);
    caretaker_number[strcspn(caretaker_number, "\n")] = '\0';  // Remove newline character

    printf("Enter Twilio Number: ");
    fgets(twilio_number, sizeof(twilio_number), stdin);
    twilio_number[strcspn(twilio_number, "\n")] = '\0';  // Remove newline character

    // Properly format and set environment variable for Twilio SID
    char env_var[256];  // Ensure the buffer is large enough
    
    memset(env_var, 0, sizeof(env_var));  // Clear the buffer before use
    sprintf(env_var, "TWILIO_ACCOUNT_SID=%s", twilio_SSID);
    putenv(env_var);  // Set the environment variable

    memset(env_var, 0, sizeof(env_var));  // Clear the buffer before use
    sprintf(env_var, "TWILIO_AUTH_TOKEN=%s", twilio_token);
    putenv(env_var);  // Set the environment variable

    memset(env_var, 0, sizeof(env_var));  // Clear the buffer before use
    sprintf(env_var, "TWILIO_NUMBER=%s", twilio_number);
    putenv(env_var);  // Set the environment variable

    memset(env_var, 0, sizeof(env_var));  // Clear the buffer before use
    sprintf(env_var, "PATIENT_NAME=%s", patient);
    putenv(env_var);  // Set the environment variable

    memset(env_var, 0, sizeof(env_var));  // Clear the buffer before use
    sprintf(env_var, "CARETAKER_NUMBER=%s", caretaker_number);
    putenv(env_var);  // Set the environment variable

    return 0;
}
