// www.elegoo.com
// 2018.10.24
/*
  LED1 should be lit, showing power. LED2 indicates sound input, and the sensitivity is adjusted by potentiometer.
  There is a tiny screw on the blue potentiometer block that you can use for adjustment. Turning that
  clockwise lowers the potentiometer value, while counter-clockwise raises the potentiometer value.
  Use the potentiometer to adjust the Sound Sensor sensitivity. Turn the potentiometer
  several rotations until you see the LED2 extinguish (or just faintly blink). This might be slightly greater than
  500, if you are also watching Serial Monitor (inital adjustment), or, Serial Plotter (the latter is prefererd for observation).
  Special thanks to user CRomer, for his input and hard work!
*/

#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

// Select the Arduino input pin to accept the Sound Sensor's analog output 
#define sensorAnalogPin 25
// Select the Arduino input pin to accept the Sound Sensor's digital output
#define sensorDigitalPin 24
#define Led13 21

int main(void) {

    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }
    int  analogValue = 0;         // Define variable to store the analog value coming from the Sound Sensor
    int  digitalValue;            // Define variable to store the digital value coming from the Sound Sensor
                              // When D0 from the Sound Sensor (connnected to pin 3 on the
                              // Arduino) sends High (voltage present), L will light. In practice, you
                              // should see LED13 on the Arduino blink when LED2 on the Sensor is 100% lit.
    
    pinMode(sensorDigitalPin,INPUT);  // Define pin 3 as an input port, to accept digital input
    pinMode(Led13,OUTPUT);            // Define LED13 as an output port, to indicate digital trigger reached

    int val = 0;

    while(1){
        analogValue = analogRead(sensorAnalogPin); // Read the value of the analog interface A0 assigned to digitalValue 
        digitalValue = digitalRead(sensorDigitalPin); // Read the value of the digital interface 3 assigned to digitalValue 
        printf("%d", analogValue); // Send the analog value to the serial transmit interface
        
        if(digitalValue==HIGH)      // When the Sound Sensor sends signla, via voltage present, light LED13 (L)
        {
          digitalWrite(Led13,HIGH);
        }
        else
        {
          digitalWrite(Led13,LOW);
        }
        
        delay(50);                  // Slight pause so that we don't overwhelm the serial interface
    }
    return 0;
}