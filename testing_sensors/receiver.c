#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

/* Send Pin */ 
#define RECEIVER_PIN 2

int main(void) {

    if(wiringPiSetup() == -1){
        //if the wiringPi initialization fails, print error message
            printf("setup wiringPi failed !");
            return 1;
        }
        pinMode(RECEIVER_PIN, INPUT);
        int val = 0;
    
        while(1){
            val = digitalRead(RECEIVER_PIN); //read the value of the digital interface 3 assigned to val
            if (val == HIGH)         //when the shock sensor have signal, LED blink
            {
                printf("Receiving 1...\n");
                delay(500);
            }
            else
            {
                printf("Receiving 0...\n");
                delay(500);
            }
        }
        return 0;
}