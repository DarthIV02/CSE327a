#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

// Select the Arduino input pin to accept the Sound Sensor's digital output
#define DETECTION 26 
#define Led13 21

int main(void) {

    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }

    int  digitalValue;            // Define variable to store the digital value coming from the Sound Sensor
                              // When D0 from the Sound Sensor (connnected to pin 3 on the
                              // Arduino) sends High (voltage present), L will light. In practice, you
                              // should see LED13 on the Arduino blink when LED2 on the Sensor is 100% lit.
    
    pinMode(DETECTION,INPUT);  // Define pin 3 as an input port, to accept digital input
    pinMode(Led13,OUTPUT);            // Define LED13 as an output port, to indicate digital trigger reached

    digitalWrite(Led13, LOW);

    while(1){
        digitalValue = digitalRead(DETECTION); // Read the value of the digital interface 3 assigned to digitalValue 
        
        if(digitalValue==HIGH)      // When the Sound Sensor sends signla, via voltage present, light LED13 (L)
        {
          digitalWrite(Led13,HIGH);
        }
        else
        {
          digitalWrite(Led13,LOW);
        }
        
        delay(100);                  // Slight pause so that we don't overwhelm the serial interface
    }
    return 0;
}