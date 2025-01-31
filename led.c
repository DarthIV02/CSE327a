#include <wiringPi.h>
#include <stdio.h>
#define LEDPinRed 0
#define LEDPinYellow 1

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
        digitalWrite(LEDPinRed, LOW); //led red off
        digitalWrite(LEDPinYellow, HIGH); //led yellow on
        printf("led off...\n");
        delay(500);
        digitalWrite(LEDPinRed, HIGH); //led red on
        digitalWrite(LEDPinYellow, LOW); //led yellow off
        printf("...led on\n");
        delay(500);
    }
    return 0;
}
