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
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// GPIO Pins, BCM numbering
#define RED_BUTTON 5

void cleanup(int signo) {
    pullUpDnControl(RED_BUTTON, PUD_DOWN);

    exit(0);
}

void goRed() {
    printf("Button pressed...\n");
    delay(100);
}

unsigned short int isPressed(unsigned short int button) {
    static struct timespec lastCall;
    struct timespec thisCall;
    float timeDiff;

    clock_gettime(CLOCK_REALTIME, &thisCall);
    timeDiff = (thisCall.tv_sec + thisCall.tv_nsec/1E9 - lastCall.tv_sec - lastCall.tv_nsec/1E9)*5;
    lastCall = thisCall;

    return timeDiff > 1 ? 1 : 0;
}

unsigned short int isHeld(unsigned short int button, unsigned short int holdTime) {
    unsigned short int sample;
    unsigned short int sampleCount = holdTime/25;
    unsigned short int delayInterval = holdTime/40;

    for(sample=0; sample<sampleCount; sample++) {
        if (digitalRead(button)) {
            break;
        }

        delay(delayInterval);
    }

    return sample == sampleCount ? 1 : 0;
}

int main(void) {
    //signal(SIGINT, cleanup);
    //signal(SIGTERM, cleanup);
    //signal(SIGHUP, cleanup);

    wiringPiSetupGpio();

    pinMode(RED_BUTTON, INPUT);
    pullUpDnControl(RED_BUTTON, PUD_UP);

    printf("Ready :)");

    wiringPiISR(RED_BUTTON, INT_EDGE_BOTH, goRed);
    
    pause();
    return 0;
}