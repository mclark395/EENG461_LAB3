//
// Created by Mark Clark on 10/3/23.
//

#include "main.h"
#include "setup.h"
#include <stdint.h>
#include <common/tm4c123gh6pm.h>

int sec_count;

int main (void) {
    sec_count = 0;
    setup();

    while (1) {
        for(uint32_t i = 0; i < 5333333; i++) {}
        sec_count++;

        /*
         * Start loop by turning on blue LED
         */
        GPIO_PORTF_DATA_R |= BLUE_LED;

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
        }
    }

    return (0);
}

