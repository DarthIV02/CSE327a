#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>
#include <string.h>

/* Selecciona el pin para el LED rojo */ 
#define CLK 24
/* Selecciona el pin para el LED verde */ 
#define DT 23
/* Selecciona el pin para el LED azul */ 
#define SW 22

int counter = 0;
int currentStateCLK;
int lastStateCLK;
unsigned long lastButtonPress = 0;

int main(void) {

    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }

    // Set encoder pins as inputs
	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	pinMode(SW, INPUT); // INPUT_PULLUP
    // Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);

    while(1){
        // Read the current state of CLK
        currentStateCLK = digitalRead(CLK);
        printf("Clock: ", currentStateCLK);

        // If last and current state of CLK are different, then pulse occurred
        // React to only 1 state change to avoid double count
        if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

            char currentDir[] = "";

            // If the DT state is different than the CLK state then
            // the encoder is rotating CCW so decrement
            if (digitalRead(DT) != currentStateCLK) {
                counter --;
                char currentDir[] = "CCW";
            } else {
                // Encoder is rotating CW so increment
                counter ++;
                char currentDir[] = "CW";
            }

            printf("Direction: %s | Counter: %d", currentDir, counter);
        }

        // Remember last CLK state
        lastStateCLK = currentStateCLK;

        // Read the button state
        int btnState = digitalRead(SW);

        //If we detect LOW signal, button is pressed
        if (btnState == LOW) {
            //if 50ms have passed since last LOW pulse, it means that the
            //button has been pressed, released and pressed again
            if (millis() - lastButtonPress > 50) {
                printf("Button pressed!");
            }

            // Remember last button press event
            lastButtonPress = millis();
        }

        delay(1);
    }

    return 0;
}