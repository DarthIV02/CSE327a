#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

/* Selecciona el pin para el LED rojo */ 
#define redpin 27
/* Selecciona el pin para el LED verde */ 
#define greenpin 28
/* Selecciona el pin para el LED azul */ 
#define bluepin 29

int main(void) {

    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }
    pinMode(redpin, OUTPUT); 
    pinMode(bluepin, OUTPUT);
    pinMode(greenpin, OUTPUT); 
    softPwmCreate(redpin, 0, 0xFF);
    softPwmCreate(bluepin, 0, 0xFF);
    softPwmCreate(greenpin, 0, 0xFF);

    int val = 0;

    while(1){
        
        softPwmWrite(redpin, val);
        softPwmWrite(greenpin, val); 
        softPwmWrite(bluepin, val); 
        delay(50);
    }
    return 0;
}