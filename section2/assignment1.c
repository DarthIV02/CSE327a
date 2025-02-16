#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

void init_shared_variable(SharedVariable* sv) {
    sv->bProgramExit = 0; // running 
    sv->detection_movement = 0; // none 
    sv->detection_sound = 0; // none
    sv->detect_direction = 0; // cw = 1 and ccw = 2
    sv->button_pressed = 1; // not pressed
    sv->ordered = 0;
// You can initialize the shared variable if needed.
}

void ledInit(void) {

// Initialize LEDs

    pinMode(PIN_SMD_RED, OUTPUT);
    pinMode(PIN_SMD_GRN, OUTPUT);
    pinMode(PIN_SMD_BLU, OUTPUT);
    softPwmCreate(PIN_SMD_RED, 0, 0xFF);
    softPwmCreate(PIN_SMD_GRN, 0, 0xFF);
    softPwmCreate(PIN_SMD_BLU, 0, 0xFF);
    
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
    pinMode(PIN_ROTARY_CLK, INPUT);
	pinMode(PIN_ROTARY_DT, INPUT);
    pinMode(PIN_BUTTON, INPUT);
}

// 1. Button
void body_button(SharedVariable* sv) {
    int read_button = READ(PIN_BUTTON);

    if (sv->bProgramExit == 2){
        // If paused then turn off
        TURN_OFF(PIN_ALED);
        TURN_OFF(PIN_DIP_GRN);
        TURN_OFF(PIN_DIP_RED);
        softPwmWrite(PIN_SMD_RED, 0);
        softPwmWrite(PIN_SMD_GRN, 0);
        softPwmWrite(PIN_SMD_BLU, 0);
    }

    if (read_button == 0 && sv->button_pressed == 1){ // change state when clicked
        if(sv->bProgramExit == 0){
            sv->bProgramExit = 2;
        } else if (sv->bProgramExit == 2){
            sv->bProgramExit = 0;
        }
    }

    sv->button_pressed = read_button; // save previous state
}

// 2. Infrared Motion Sensor
void body_motion(SharedVariable* sv) {
    sv->detection_movement = READ(PIN_MOTION); // Read the value of the movement
}

// 3. Microphone Sound Sensor
void body_sound(SharedVariable* sv) {
    sv->detection_sound = READ(PIN_SOUND); // Read the value of the microphone
}

// 4. Rotary Encoder
void body_encoder(SharedVariable* sv) {
    // Read the current state of CLK
    sv->current_click = READ(PIN_ROTARY_CLK);

    // If last and current state of CLK are different, then pulse occurred
    // React to only 1 state change to avoid double count
    if (sv->current_click != sv->past_click  && sv->current_click == 1){

        // If the DT state is different than the CLK state then
        // the encoder is rotating CCW so decrement
        if (READ(PIN_ROTARY_DT) != sv->current_click) { 
            // counterclockwise
            sv->detect_direction = 0;
        } else {
            // clockwise
            sv->detect_direction = 1;
        }
    }

    // Remember last CLK state
    sv->past_click = sv->current_click;
}

// 5. DIP two-color LED
void body_twocolor(SharedVariable* sv) {
    if (sv->bProgramExit == 0){ // Make sure to only turn on when running
        if(sv->detection_movement == 1){ // If it detects movement
            TURN_ON(PIN_DIP_GRN);
            TURN_OFF(PIN_DIP_RED);
        } else { // No movement
            TURN_ON(PIN_DIP_RED);
            TURN_OFF(PIN_DIP_GRN);
        }
    }
}

// 6. SMD RGB LED
void body_rgbcolor(SharedVariable* sv) {
    if (sv->bProgramExit == 0){ // If the program is running
        if(sv->detect_direction == 0){ // Counterclockwise
            if(sv->detection_movement == 0){ // No motion
                softPwmWrite(PIN_SMD_RED, 0xEE);
                softPwmWrite(PIN_SMD_GRN, 0x00);
                softPwmWrite(PIN_SMD_BLU, 0xC8);
            } else {  // Motion
                softPwmWrite(PIN_SMD_RED, 0x00);
                softPwmWrite(PIN_SMD_GRN, 0xFF);
                softPwmWrite(PIN_SMD_BLU, 0xFF);    
            }
        } else { // Clockwise
            if(sv->detection_movement == 0){ // No motion
                softPwmWrite(PIN_SMD_RED, 0xFF);
                softPwmWrite(PIN_SMD_GRN, 0x00);
                softPwmWrite(PIN_SMD_BLU, 0x00);
            } else { // Motion
                softPwmWrite(PIN_SMD_RED, 0x80);
                softPwmWrite(PIN_SMD_GRN, 0xFF);
                softPwmWrite(PIN_SMD_BLU, 0x00);    
            }
        }
    }
}

// 7. Auto-flash LED
void body_aled(SharedVariable* sv) {
    if (sv->bProgramExit == 0){ // If running
        TURN_ON(PIN_ALED); //led on
    }
}

void playTone(int frequency, int duration) {
    int delayTime = 1000000 / frequency / 2;  // Half period in microseconds
    int cycles = frequency * duration / 1000;  // Number of cycles

    unsigned long start_time;

    for (int i = 0; i < cycles; i++) {
        TURN_ON(PIN_BUZZER);
        //delayMicroseconds(delayTime);
        start_time = millis();
        while (millis() - start_time < delayTime / 100) {  // Convert microseconds to milliseconds
            // Do nothing, just wait
        }
        TURN_OFF(PIN_BUZZER);
        //delayMicroseconds(delayTime);
        start_time = millis();
        while (millis() - start_time < delayTime / 100) {  // Convert microseconds to milliseconds
            // Do nothing, just wait
        }
    }
}

// 8. Buzzer
void body_buzzer(SharedVariable* sv) {
    if (sv->detection_sound == 1){
        playTone(3136, 300); //Play for 3 seconds
    }
}