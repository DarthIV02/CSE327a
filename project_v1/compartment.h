
#ifndef _ASSIGNMENT_BODY_
#define _ASSIGNMENT_BODY_

#include <stdint.h>

// Macros
#define TURN_ON(pin) digitalWrite(pin, 1)
#define TURN_OFF(pin) digitalWrite(pin, 0)

#define READ(pin) digitalRead(pin)
#define WRITE(pin, x) digitalWrite(pin, x)

// Constants
#define HELP 3
#define NO_HELP 2

#define LOW 0
#define HIGH 1


#define DETECT_SOUND 1
#define NO_SOUND 0

// A. Pin number definitions (DO NOT MODIFY)
// We use 8 sensors.
//1. Snooze Button
#define PIN_BUTTON 0

// 2. Infrared motion sensor + Buzzer
#define TIME_BEFORE_ALARM 10000
#define PIN_MOTION 4
#define PIN_BUZZER 15


// 5. DIP two-color LED (Dual In-line Package)
#define PIN_DIP_RED 5
#define PIN_DIP_GRN 6

// Clock Pins
// SCL BCM 3 (9)
// SDA BCM 2 (8)


// 9. DC MOTOR
// Encoder Pins
#define C1 21
#define C2 22

// 10. MOTOR Driver
#define PWM_MOTOR_ENABLE 26
#define DRIVER_IN1 28
#define DRIVER_IN2 29

//#define kp .0001
//#define ki .0001
//#define kd .02

#define kp .5
#define ki .0
#define kd .1

// PPR = 1050
// 900 seems to work i guess
#define CONVERSION_RATIO 360/1050

#define CONTAINER_0_POSITION 0
#define CONTAINER_1_POSITION 180

// 11. Servo Motors
#define SERVO_0 23
#define SERVO_1 24

// B. Shared structure
// All thread functions get a shared variable of the structure
// as the function parameter.
// If needed, you can add anything in this structure.
typedef struct shared_variable {
    int bProgramExit; // Once set to 1, the program will terminate.

    // Sensor data
    // IR MOTION SENSOR VARS
    //int motion_data;
    //int last_motion_data;
    //int time_waiting_to_take_medicine;
    //int medicine_present;
    
    // DC MOTOR VARS
    int c1_data;
    int c2_data;
    int position_of_compartment_1;
    
    

} SharedVariable;


// C. Functions
// You need to implement the following functions.
// Do not change any function name here.
void init_shared_variable(SharedVariable* sv);
void init_sensors(SharedVariable* sv);

void body_motion(SharedVariable* sv);     // Infrared motion sensor
void medicine_dropped();

void body_container(SharedVariable* sv);      // Microphone sound sensor
void read_encoder();
void setMotor(int dir, int pwm_val, int pwm_pin, int in1, int in2);
void goToContainer(int container);
void openServo(int container);
void takeTheDamnMedicine();
void snooze_button();
#endif
