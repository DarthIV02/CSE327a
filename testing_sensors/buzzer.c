#include <wiringPi.h>
#include <stdio.h>
#include "pitches.h"  // Ensure this file contains correct frequency values

/* Define Buzzer Pin */
#define BUZZER 13

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
    int melody[] = {
        NOTE_AS4, NOTE_AS4, NOTE_AS4,
        NOTE_F5, NOTE_C6,
        NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
        NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
        NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5,
        NOTE_F5, NOTE_C6,
        NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,

        NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
        NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5,
        NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
        NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
        NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,

        NOTE_C6, NOTE_G5, NOTE_G5, REST, NOTE_C5,
        NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
        NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C6, NOTE_C6,
        NOTE_F6, NOTE_DS6, NOTE_CS6, NOTE_C6, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5,
        NOTE_C6
    };
    int durations[] = {
        8, 8, 8,
        2, 2,
        8, 8, 8, 2, 4,
        8, 8, 8, 2, 4,
        8, 8, 8, 2, 8, 8, 8,
        2, 2,
        8, 8, 8, 2, 4,

        8, 8, 8, 2, 4,
        8, 8, 8, 2, 8, 16,
        4, 8, 8, 8, 8, 8,
        8, 8, 8, 4, 8, 4, 8, 16,
        4, 8, 8, 8, 8, 8,

        8, 16, 2, 8, 8,
        4, 8, 8, 8, 8, 8,
        8, 8, 8, 4, 8, 4, 8, 16,
        4, 8, 4, 8, 4, 8, 4, 8,
        1
    };

    int size = sizeof(durations) / sizeof(int);

    while (1) {
        for (int thisNote = 0; thisNote < size; thisNote++) {
            printf("Playing frequency: %d Hz\n", melody[thisNote]);
            int duration = 1000 / durations[thisNote];
            playTone(melody[thisNote], duration);
            int pauseBetweenNotes = duration * 1.30;
            delay(pauseBetweenNotes); // Small gap between notes
        }
        delay(2000);  // Pause before repeating
    }

    return 0;
}
