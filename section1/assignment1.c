#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

void init_shared_variable(SharedVariable* sv) {
    sv->bProgramExit = 0;
    sv->detection_movement = 0;
    sv->detection_sound = 0;
// You can initialize the shared variable if needed.
}

void ledInit(void) {

// Initialize LEDs

    softPwmCreate(PIN_SMD_RED, 0, 0xff);
    pinMode(PIN_DIP_RED, OUTPUT);
    pinMode(PIN_DIP_GRN, OUTPUT);
    pinMode(PIN_ALED, OUTPUT);
}

void init_sensors(SharedVariable* sv) {
// .......
    ledInit();
    pinMode(PIN_MOTION, INPUT);
    pinMode(PIN_SOUND, INPUT);
    pinMode(PIN_BUZZER, OUTPUT);
}

// 1. Button
void body_button(SharedVariable* sv) {
}

// 2. Infrared Motion Sensor
void body_motion(SharedVariable* sv) {
    sv->detection_movement = !READ(PIN_MOTION); // Read the value of the movement
}

// 3. Microphone Sound Sensor
void body_sound(SharedVariable* sv) {
    sv->detection_sound = READ(PIN_SOUND); // Read the value of the microphone
}

// 4. Rotary Encoder
void body_encoder(SharedVariable* sv) {
}

// 5. DIP two-color LED
void body_twocolor(SharedVariable* sv) {
    if(sv->detection_movement == 0){
        TURN_ON(PIN_DIP_GRN);
        TURN_OFF(PIN_DIP_RED);
    } else {
        TURN_ON(PIN_DIP_RED);
        TURN_OFF(PIN_DIP_GRN);
    }
}

// 6. SMD RGB LED
void body_rgbcolor(SharedVariable* sv) {
}

// 7. Auto-flash LED
void body_aled(SharedVariable* sv) {
    TURN_ON(PIN_ALED); //led on
}

void playTone(int frequency, int duration) {
    int delayTime = 1000000 / frequency / 2;  // Half period in microseconds
    int cycles = frequency * duration / 1000;  // Number of cycles

    unsigned long start_time;

    for (int i = 0; i < cycles; i++) {
        TURN_ON(PIN_BUZZER);
        //delayMicroseconds(delayTime);
        start_time = millis();
        while (millis() - start_time < delayTime) {  // Convert microseconds to milliseconds
            // Do nothing, just wait
        }
        TURN_OFF(PIN_BUZZER);
        //delayMicroseconds(delayTime);
        start_time = millis();
        while (millis() - start_time < delayTime) {  // Convert microseconds to milliseconds
            // Do nothing, just wait
        }
    }
}

// 8. Buzzer
void body_buzzer(SharedVariable* sv) {
    if (sv->detection_sound == 1){
        playTone(2093, 3000); //Play for 3 seconds
    }
}
