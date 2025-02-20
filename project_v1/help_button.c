/*

    Program: 	wiringPi Buttons (buttons.c)
    Author:  M. Heidenreich, (c) 2021

    Description:
    This code is provided in support of the following YouTube tutorial:
    https://www.youtube.com/watch?v=9Znf0CnsbXI 
    
    This 2-part Raspberry Pi GPIO tutorial shows how to correctly use buttons with wiringPi.
    While I use the buttons to control LEDs, you can use the same approach with any other
    electronic components (or code in a subroutine/function in general).

    THIS SOFTWARE AND LINKED VIDEO TOTORIAL ARE PROVIDED "AS IS" AND THE
    AUTHOR DISCLAIMS ALL WARRANTIES INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <wiringPi.h>
#include <stdio.h>

// GPIO Pins, BCM numbering
#define BUTTON 7

int main(void) {

    if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
        printf("setup wiringPi failed !");
        return 1;
    }
    pinMode(BUTTON, INPUT);
    int val = 0;

    while(1){
        val = digitalRead(BUTTON); //read the value of the digital interface 3 assigned to val
        if (val == HIGH)         //when the shock sensor have signal, LED blink
        {
            printf("button not pressed...\n");
            delay(50);
        }
        else
        {
            printf("button pressed...\n");
            delay(50);
        }
    }
    return 0;
}