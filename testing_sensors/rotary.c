#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

/* Selecciona el pin para el LED rojo */ 
#define CLK 24
/* Selecciona el pin para el LED verde */ 
#define DT 23
/* Selecciona el pin para el LED azul */ 
#define SW 22

int count = 0;//Define the count
int lastCLK = 0;//CLK initial value

int main(void) {

    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }

    const int interrupt0 = 0;// Interrupt 0 在 pin 2 上

    pinMode(SW, INPUT);
    digitalWrite(SW, HIGH);
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    attachInterrupt(interrupt0, ClockChanged, CHANGE);//Set the interrupt 0 handler, trigger level change

    while(1){
        if (!digitalRead(SW) && count != 0) //Read the button press and the count value to 0 when the counter reset
        {
            count = 0;
            printf("count: %d", count);
        }
    }

    return 0;
}

//The interrupt handlers
void ClockChanged()
{
  int clkValue = digitalRead(CLK);//Read the CLK pin level
  int dtValue = digitalRead(DT);//Read the DT pin level
  if (lastCLK != clkValue)
  {
    lastCLK = clkValue;
    count += (clkValue != dtValue ? 1 : -1);//CLK and inconsistent DT + 1, otherwise - 1

    printf("count: %d", count);
  }
}