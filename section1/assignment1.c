#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

void init_shared_variable(SharedVariable* sv) {
    sv->bProgramExit = 0;
    sv->detection = 0;
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
}

// 1. Button
void body_button(SharedVariable* sv) {
}

// 2. Infrared Motion Sensor
void body_motion(SharedVariable* sv) {
    sv->detection = !READ(PIN_MOTION); // Read the value of the digital interface 3 assigned to digitalValue
}

// 3. Microphone Sound Sensor
void body_sound(SharedVariable* sv) {
}

// 4. Rotary Encoder
void body_encoder(SharedVariable* sv) {
}

// 5. DIP two-color LED
void body_twocolor(SharedVariable* sv) {
    if(sv->detection == 0){
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

// 8. Buzzer
void body_buzzer(SharedVariable* sv) {
}
