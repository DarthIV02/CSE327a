#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

/* Send Pin */ 
#define OUTPUT_PIN 0

int main(void) {

    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }
    pinMode(OUTPUT_PIN, OUTPUT);
    softPwmCreate(OUTPUT_PIN, 0, 1);

    while(1){
        /* send 0x0 code (8 bits)*/ 
        softPwmWrite(OUTPUT_PIN, 0);
        Delay (200); 
        softPwmWrite(OUTPUT_PIN, 1);
        Delay (200);
    }
    return 0;
}