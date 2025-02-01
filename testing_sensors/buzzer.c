#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>
#include "pitches.h"

/* Selecciona el pin para el Buzzer */ 
#define BUZZER 3



int main(void) {

    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }

    pinMode(BUZZER, OUTPUT);
    softPwmCreate(BUZZER, 0, 5000);

    // notes in the melody:
    int melody[] = {NOTE_C7, NOTE_D7, NOTE_E7, NOTE_FS7, NOTE_GS7, NOTE_AS7, NOTE_C8, NOTE_D8};
    int duration = 500;  // 500 miliseconds

    while(1){
        for (int thisNote = 0; thisNote < 8; thisNote++) {
        // pin8 output the voice, every scale is 0.5 sencond
            softPwmWrite(BUZZER, melody[thisNote]);

            printf("Melody: %d", melody[thisNote])
            
            // Output the voice after several minutes
            delay(duration);
        }
        
        // restart after two seconds 
        delay(2000);
    }

    return 0;
}