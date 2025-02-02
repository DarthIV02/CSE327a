#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

void init_shared_variable(SharedVariable* sv) {
    sv->bProgramExit = 0;
// You can initialize the shared variable if needed.
}

void ledInit(void) {
    softPwmCreate(PIN_SMD_RED, 0, 0xff);

//......
//initialize SMD and DIP
}

void init_sensors(SharedVariable* sv) {
// .......
    ledInit();
    pinMode(PIN_ALED, OUTPUT);
}

// 1. Button
void body_button(SharedVariable* sv) {
}

// 2. Infrared Motion Sensor
void body_motion(SharedVariable* sv) {
}

// 3. Microphone Sound Sensor
void body_sound(SharedVariable* sv) {
}

// 4. Rotary Encoder
void body_encoder(SharedVariable* sv) {
}

// 5. DIP two-color LED
void body_twocolor(SharedVariable* sv) {
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
