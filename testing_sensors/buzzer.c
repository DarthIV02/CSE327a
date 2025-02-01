#include <wiringPi.h>
#include <stdio.h>
#include "pitches.h"  // Ensure this file contains correct frequency values

/* Define Buzzer Pin */
#define BUZZER 3

void playTone(int frequency, int duration) {
    int delayTime = 1000000 / frequency / 2;  // Half period in microseconds
    int cycles = frequency * duration / 1000;  // Number of cycles

    for (int i = 0; i < cycles; i++) {
        digitalWrite(BUZZER, HIGH);
        delayMicroseconds(delayTime);
        digitalWrite(BUZZER, LOW);
        delayMicroseconds(delayTime);
    }
}

int main(void) {
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed!\n");
        return 1;
    }

    pinMode(BUZZER, OUTPUT);

    // Melody notes
    int melody[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
    int duration = 500;  // Each note duration in milliseconds

    while (1) {
        for (int thisNote = 0; thisNote < 8; thisNote++) {
            printf("Playing frequency: %d Hz\n", melody[thisNote]);
            playTone(melody[thisNote], duration);
            delay(50);  // Small gap between notes
        }
        delay(2000);  // Pause before repeating
    }

    return 0;
}
