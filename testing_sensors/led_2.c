#include <wiringPi.h>
#include <stdio.h>
#define LEDPinRed 8
#define LEDPinYellow 9

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
        printf("led red...\n");
        delay(500);
        digitalWrite(LEDPinRed, HIGH); //led red on
        digitalWrite(LEDPinYellow, LOW); //led yellow off
        printf("...led yellow\n");
        delay(500);
    }
    return 0;
}
