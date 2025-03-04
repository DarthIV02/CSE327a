// HELP:
// Button changes rapidly for a button press with debouncing
// Encoder not working properly

#include "compartment.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <time.h>
#include <math.h>

// idk where else to put these
// time start for IR Motion sensor
int time_start = 0;

// global position
int global_pos = 0;
//PID vars
int previous_time = 0;
float e_prev = 0;
float e_integral = 0;

void init_shared_variable(SharedVariable* sv) {
    sv->bProgramExit = 0;

    //  IR MOTION VARS
    sv->motion_data = 0;
    sv->last_motion_data = 0;
    sv->time_waiting_to_take_medicine = 0;
    sv->medicine_present = 0;
    
    // DC MOTOR VARS
    sv->c1_data = 0;
    sv->c2_data = 0;
    global_pos = 0;
// You can initialize the shared variable if needed.
}

void ledInit(void) {
    softPwmCreate(PIN_DIP_GRN, 0, 0xff);                // set up PWM for PIN_SMD_GRN
    softPwmCreate(PIN_DIP_RED, 0, 0xff);                // set up PWM for PIN_SMD_RED
}

void init_sensors(SharedVariable* sv) {
// .......
    ledInit();
    pinMode(PIN_MOTION, INPUT);
    // DC Motor Pins
    pinMode(C1, INPUT);
    pinMode(C2, INPUT);
    pinMode(DRIVER_IN1, OUTPUT);
    pinMode(DRIVER_IN2, OUTPUT);
    softPwmCreate(PWM_MOTOR_ENABLE, 0, 0x00);
    //pinMode(PWM_MOTOR_ENABLE, PWM_OUTPUT);
    wiringPiISR(C1, INT_EDGE_RISING, read_encoder);
    wiringPiISR(PIN_MOTION, INT_EDGE_RISING, medicine_dropped);
    // Servo Motor Pins
    //softPwmCreate(SERVO_0, 0, 0x00);
    //softPwmCreate(SERVO_1, 0, 0x00);
    pinMode(SERVO_0, PWM_OUTPUT);
    pinMode(SERVO_1, PWM_OUTPUT);
}

// 2. Infrared Motion Sensor - 0 = blocked, 1 = not blocked
void body_motion(SharedVariable* sv) {
    sv->motion_data = digitalRead(PIN_MOTION);
    if(sv->last_motion_data == 0 && sv->motion_data == 0)                   // medicine remains in front of sensor, keeping track of how much time it has been there
    {
        sv->time_waiting_to_take_medicine = millis() - time_start;
        sv->medicine_present = 1;
    }
    else                                                                    // medicine gets taken from the sensor or theres nothing present in front of the sensor
    {
        sv->medicine_present = 0;
        sv->time_waiting_to_take_medicine = 0;
        time_start = 0;
    }
    if(sv->medicine_present == 1)
    {
        printf("MEDICINE PRESENT! Time elapsed: %d\n", sv->time_waiting_to_take_medicine);
    }
    else 
    {
        
    }

}

void medicine_dropped()
{
    time_start = millis();
}

// 9. DC Motor - Signal seems to be going out of the wire and into the input pin of the motor driver, but isn't getting sent to the output pins of the driver. This happens when switching to another motor driver too
void body_container(SharedVariable* sv) {
    // DEBUG POSITION READING
    //digitalWrite(DRIVER_IN1, HIGH);
    //digitalWrite(DRIVER_IN2, LOW);
    //sv->position_of_compartment_1 = global_pos*CONVERSION_RATIO;
    //printf("%d\n", sv->position_of_compartment_1);
    
    // DEBUG MOTOR CW/CCW MOVEMENT 
    //setMotor(1, 0xff, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    //sv->position_of_compartment_1 = global_pos*CONVERSION_RATIO;
    //printf("%d\n", sv->position_of_compartment_1);
    //delay(2000);
    
    //setMotor(-1, 0x19, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    //sv->position_of_compartment_1 = global_pos*CONVERSION_RATIO;
    //printf("%d\n", sv->position_of_compartment_1);
    //delay(2000);
    
    //setMotor(0, 0x19, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    //sv->position_of_compartment_1 = global_pos*CONVERSION_RATIO;
    //printf("%d\n", sv->position_of_compartment_1);
    //delay(2000);
    
    // DEBUG PID
    goToContainer(0);
    int test_target = CONTAINER_0_POSITION;                                 // can be either container 0 or container 1
    printf("Current Position: %d, Target: %d\n", global_pos*CONVERSION_RATIO, test_target);
    
    
}


void read_encoder()
{
    int other = digitalRead(C2);
    if(other > 0)
    {
        global_pos--;
    }
    else 
    {
        global_pos++;
    }
}

void setMotor(int dir, int pwm_val, int pwm_pin, int in1, int in2)
{
    softPwmWrite(PWM_MOTOR_ENABLE, pwm_val);
    if(dir == 1)                    // CLOCKWISE from motor POV
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    }
    else if(dir == -1)              // COUNTERCLOCKWISE from motor POV
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
    else 
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    }
}

// Move to container through PID, right now takes in the target degree
void goToContainer(int container)
{
    // position
    int current_position = global_pos*CONVERSION_RATIO;
    int target;
    
    if(container == 0)
    {
        target = 0;
    }
    else if(container == 1)
    {
        target = 180;
    }
    if(current_position == target)          // hard-coded solution to PID problem LMAO
    {
        setMotor(0, 0x0, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
        openServo(container);
        return;
    }
    
    // time difference
    int current_time = millis();
    float delta_time = (current_time - previous_time); // change in time in seconds
    previous_time = current_time;
    // error
    int e = current_position - target;
    // derivative and integral calculation
    float dedt = (float)(e-e_prev)/delta_time;
    e_integral = (float)(e_integral + e*delta_time);
    
    // control signal
    float output_signal = (float)(kp*e + kd*dedt + ki*e_integral);
    
    // Compute pwm signal
    float pwr = fabs(output_signal);
    if(pwr > 255)
    {
        pwr = 255;
    }
    int dir = 1;
    if(output_signal < 0)
    {
        dir = -1;
    }
    setMotor(dir, pwr, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    e_prev = e;
}

void openServo(int container)
{
    if(container == 0)
    {
        printf("Bruh0 \n");
        pwmWrite(SERVO_0, 0);
        delay(1000);
        printf("Bruh1 \n");
        pwmWrite(SERVO_0, 512);
        delay(1000);
        printf("Bruh2 \n");
        pwmWrite(SERVO_0, 1024);
        delay(1000);
    }
    if(container == 1)
    {
        printf("Bruh0 \n");
        pwmWrite(SERVO_1, 0);
        delay(1000);
        printf("Bruh1 \n");
        pwmWrite(SERVO_1, 512);
        delay(1000);
        printf("Bruh2 \n");
        pwmWrite(SERVO_1, 1024);
        delay(1000);
    }
}



