//
// Created by Mark Clark on 10/3/23.
//

#include "main.h"
#include "setup.h"
#include "timers.h"
#include <stdint.h>
#include <common/tm4c123gh6pm.h>

volatile int sec_count = 0; //Global Variable so timerISR can increment it

int main (void) {

    setup();
    configureTimer();

    Enable_Interrupts(); //Enable Global Interrupts

    /*
     * Start loop by turning on blue LED
     */
    GPIO_PORTF_DATA_R |= BLUE_LED;

    while (1) {

        /*
         * After 1 second of blue LED being on, turn on the red LED (leaving blue LED on)
         */
        if ((GPIO_PORTF_DATA_R & BLUE_LED) && sec_count == 1) {
            GPIO_PORTF_DATA_R |= RED_LED;
        }

        /*
         * After 2 seconds of red and blue LEDs being on, turn both of them off
         */
        if ((GPIO_PORTF_DATA_R & (BLUE_LED | RED_LED)) == (BLUE_LED | RED_LED) && sec_count == 3) {
            GPIO_PORTF_DATA_R &= ~(BLUE_LED | RED_LED); //Clear two LEDs
        }

        /*
         * Reset seconds counter to restart LED loop
         */
        if (sec_count == 5) {
            sec_count = 0;
            GPIO_PORTF_DATA_R |= BLUE_LED;
        }
    }

    return (0);
}

/*
 * Taken from Lab Assignment
 */
void Disable_Interrupts(void) {
    __asm ("  CPSID    I\n"
           " BX LR\n");
}

void Enable_Interrupts(void) {
    __asm ("  CPSIE    I\n"
           "  BX       LR\n");
}
