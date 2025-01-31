#include <wiringPi.h>
#include <stdio.h>
#define LEDPinRed 0
#define LEDPinYellow 2

int main(void)
{
    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }
    pinMode(LEDPinRed, OUTPUT);
    pinMode(LEDPinYellow, OUTPUT);
    while(1){

        digitalWrite(LEDPinRed, HIGH); // Red LED is switched on
        digitalWrite(LEDPinYellow, LOW); // green LED is switched off
        delay(3000); // Wait for 3 seconds
        
        digitalWrite(LEDPinRed, LOW); // Red LED is switched off
        digitalWrite(LEDPinYellow, HIGH); // green LED is switched on
        delay(3000); // Wait for a further 3 seconds in which the LEDs are then switched over
    }
    return 0;
}
