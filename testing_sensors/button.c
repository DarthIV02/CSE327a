#include <wiringPi.h>
#include <stdio.h>
#define  ButtonPin 5

int value; // Temporary variable
  
int main(void)
{
    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }

    pinMode(button, INPUT); // Initialization sensor pin
    digitalWrite(button, HIGH); // Activation of internal pull-up resistor
    Serial.begin(9600); // Initialization of the serial monitor
    Serial.println("KY-004 Button test");
    
    while(1){
        // The current signal at the sensor is read out.
        value = digitalRead(button); 
        // If a signal could be detected, this is displayed on the serial monitor.
        if (value == LOW) {
            Serial.println("Signal detected");
            delay(300); // 300 ms break
        }
    }
    return 0;
}