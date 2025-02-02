#include <wiringPi.h>
#include <stdio.h>
#define LEDPinRed 8
#define LEDPinYellow 9
#define LEDPFlash 12

int main(void)
{
    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }
    pinMode(LEDPinRed, OUTPUT);
    pinMode(LEDPinYellow, OUTPUT);
    pinMode(LEDPFlash, OUTPUT);
    
    digitalWrite(LEDPinRed, LOW); //led red off
    digitalWrite(LEDPinYellow, LOW); //led yellow off
    digitalWrite(LEDPFlash, LOW); //led flash off
    
    return 0;
}
