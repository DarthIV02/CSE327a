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
int snooze_button_value = 0;
// global position
int global_pos = 0;
//PID vars
int previous_time = 0;
float e_prev = 0;
float e_integral = 0;

void init_shared_variable(SharedVariable* sv) {
    sv->bProgramExit = 0;

    //  IR MOTION VARS
    // sv->motion_data = 0;
    // sv->last_motion_data = 0;
    // sv->time_waiting_to_take_medicine = 0;
    // sv->medicine_present = 0;
    
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
    // MEDICINE ALARM
    pinMode(PIN_MOTION, INPUT);
    pinMode(PIN_BUTTON, INPUT);
    
    // DC Motor Pins
    pinMode(C1, INPUT);
    pinMode(C2, INPUT);
    pinMode(DRIVER_IN1, OUTPUT);
    pinMode(DRIVER_IN2, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    //softPwmCreate(PWM_MOTOR_ENABLE, 0, 0x00);
    pinMode(PWM_MOTOR_ENABLE, PWM_OUTPUT);
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
    //sv->motion_data = digitalRead(PIN_MOTION);
    //if(sv->last_motion_data == 0 && sv->motion_data == 0)                   // medicine remains in front of sensor, keeping track of how much time it has been there
    //{
        //sv->time_waiting_to_take_medicine = millis() - time_start;
        //sv->medicine_present = 1;
    //}
    //else                                                                    // medicine gets taken from the sensor or theres nothing present in front of the sensor
    //{
        //sv->medicine_present = 0;
        //sv->time_waiting_to_take_medicine = 0;
        //time_start = 0;
    //}
    //if(sv->medicine_present == 1)
    //{
        //printf("MEDICINE PRESENT! Time elapsed: %d\n", sv->time_waiting_to_take_medicine);
    //}
    //else 
    //{
        
    //}

}

void medicine_dropped()
{
    time_start = millis();
}

// 9. DC Motor - Signal seems to be going out of the wire and into the input pin of the motor driver, but isn't getting sent to the output pins of the driver. This happens when switching to another motor driver too
void body_container(SharedVariable* sv) {
    //// DEBUG POSITION READING
    //digitalWrite(DRIVER_IN1, HIGH);
    //digitalWrite(DRIVER_IN2, LOW);
    //sv->position_of_compartment_1 = global_pos*CONVERSION_RATIO;
    //printf("%d\n", sv->position_of_compartment_1);
    
    //// DEBUG MOTOR CW/CCW MOVEMENT 
    //setMotor(1, 0xaa, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    //sv->position_of_compartment_1 = global_pos*CONVERSION_RATIO;
    //printf("%d\n", sv->position_of_compartment_1);
    //delay(2000);
    
    //setMotor(-1, 0xaa, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    //sv->position_of_compartment_1 = global_pos*CONVERSION_RATIO;
    //printf("%d\n", sv->position_of_compartment_1);
    //delay(2000);
    
    //setMotor(0, 0xaa, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    //sv->position_of_compartment_1 = global_pos*CONVERSION_RATIO;
    //printf("%d\n", sv->position_of_compartment_1);
    //delay(2000);
    
    //// DEBUG PID
    // goToContainer(1);

    //int test_target = CONTAINER_1_POSITION;                                 // can be either container 0 or container 1
    //printf("Current Position: %d, Target: %d\n", global_pos*CONVERSION_RATIO, test_target);
    // goToContainer(0);
    //test_target = CONTAINER_1_POSITION;                                 // can be either container 0 or container 1
    //printf("Current Position: %d, Target: %d\n", global_pos*CONVERSION_RATIO, test_target);
    
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
    pwmWrite(PWM_MOTOR_ENABLE, pwm_val);
    //softPwmWrite(PWM_MOTOR_ENABLE, pwm_val);
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
    setMotor(0, 0x00, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    delay(500);
    int current_position = global_pos*CONVERSION_RATIO;
    int target;
    if(container == 0)
    {
        printf("Entered 0\n");
        target = 0;
    }
    else if(container == 1)
    {
        printf("Entered 1\n");
        target = 180;
    }
    // old while argument: current_position != target
    int e = current_position - target;
    while(e != 0)
    {
        current_position = global_pos*CONVERSION_RATIO;
        // time difference
        int current_time = millis();
        float delta_time = (current_time - previous_time); // change in time in seconds
        previous_time = current_time;
        // error
        e = current_position - target; // 180 -> 0, e == 180
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
        //int dir = 1;
        int dir;
        if(e > 0)
        {
            dir = -1;
        }
        else
        {
            dir = 1;
        }
        printf("current position: %d target: %d\n", current_position, target);
        setMotor(dir, pwr, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
        e_prev = e;
    }
    setMotor(0, 0x0, PWM_MOTOR_ENABLE, DRIVER_IN1, DRIVER_IN2);
    openServo(container);
    takeTheDamnMedicine();
    return;
}

//returnToOrigin()
//{
    //goToContainer(0);
//}

void openServo(int container)
{
    if(container == 0)
    {
        printf("Bruh0 \n");
        pwmWrite(SERVO_0, 0);
        delay(1000);
        printf("Bruh0 \n");
        pwmWrite(SERVO_0, 512);
        delay(1000);
        printf("Bruh0 \n");
        pwmWrite(SERVO_0, 1024);
        delay(1000);
    }
    if(container == 1)
    {
        printf("Bruh1 \n");
        pwmWrite(SERVO_1, 0);
        delay(1000);
        printf("Bruh1 \n");
        pwmWrite(SERVO_1, 512);
        delay(1000);
        printf("Bruh1 \n");
        pwmWrite(SERVO_1, 1024);
        delay(1000);
    }
}

void takeTheDamnMedicine()
{
    int motion_data = 0;
    int last_motion_data = 0;
    int time_waiting_to_take_medicine = 0;
    int medicine_present = 0;
    int red_val, green_val;
    do
    {
        motion_data = digitalRead(PIN_MOTION);
        //printf("%d\n", motion_data);
        if(last_motion_data == 0 && motion_data == 0)                   // medicine remains in front of sensor, keeping track of how much time it has been there
        {
            time_waiting_to_take_medicine = millis() - time_start;
            medicine_present = 1;
        }
        else                                                                    // medicine gets taken from the sensor or theres nothing present in front of the sensor
        {
            medicine_present = 0;
            time_waiting_to_take_medicine = 0;
            time_start = 0;
        }
        if(medicine_present == 1)
        {
            printf("MEDICINE PRESENT! Time elapsed: %d\n", time_waiting_to_take_medicine);
            softPwmWrite(PIN_DIP_RED, 0xff);
            softPwmWrite(PIN_DIP_GRN, 0x00);
            snooze_button();
            if((time_waiting_to_take_medicine > TIME_BEFORE_ALARM) && snooze_button_value)
            {
                softPwmWrite(PIN_DIP_RED, 0x00);
                softPwmWrite(PIN_DIP_GRN, 0x00);
                for (int i = 0; i <100; i++) // Wen a frequency sound
                {
                    digitalWrite (PIN_BUZZER, HIGH) ;
                    delay (1) ;// Delay 1ms
                    digitalWrite (PIN_BUZZER, LOW) ;
                    delay (1) ;// delay ms
                }
                softPwmWrite(PIN_DIP_RED, 0xff);
                softPwmWrite(PIN_DIP_GRN, 0x00);
                delay(100);
            }
        }
        else 
        {
            softPwmWrite(PIN_DIP_RED, 0x00);
            softPwmWrite(PIN_DIP_GRN, 0x00);
            break;
        }
    }while(1);
    
}

void snooze_button() {
    int button_signal;
    unsigned long last_debounce_time = 0;
    const unsigned long debounce_delay = 500; // 50 ms debounce delay
    button_signal = digitalRead(PIN_BUTTON);
    unsigned long current_time = millis();
    if (!button_signal && (current_time - last_debounce_time > debounce_delay)) {
        last_debounce_time = current_time;
        if (snooze_button_value == 0) 
        {
            snooze_button_value = 1;
        } 
        else 
        {
            snooze_button_value = 0;
        }
    }
    
}


