//
// Created by Mark Clark on 10/3/23.
//

#include "main.h"
#include "setup.h"
#include "timers.h"
#include <stdint.h>
#include <common/tm4c123gh6pm.h>

volatile int sec_count;

int main (void) {

    setup();
    configureTimer();

    Enable_Interrupts(); //Enable Global Interrupts

    while (1) {
        switch (sec_count) {
            case 0:
                /*
                 * Time = 0 starts with blue led turning on
                 */
                *(volatile uint32_t *)((uint32_t)GPIO_PORTF_DATA_BITS_R | (uint32_t)(BLUE_LED << 2)) = BLUE_LED;
                break;
            case 1:
                /*
                 * After 1 second of blue LED being on, turn on the red LED (leaving blue LED on)
                 */
                *(volatile uint32_t *)((uint32_t)GPIO_PORTF_DATA_BITS_R | (uint32_t)(RED_LED << 2)) = RED_LED;
                break;
            case 3:
                /*
                 * After 2 seconds of red and blue LEDs being on, turn both of them off
                 */
                *(volatile uint32_t *)((uint32_t)GPIO_PORTF_DATA_BITS_R | (uint32_t)((RED_LED | BLUE_LED) << 2)) =  0;
                break;
        }
    }

    return (0);
}

/*
 * Taken from Lab Assignment
 */
void Disable_Interrupts(void) {
    __asm ("  CPSID    I\n");
}

void Enable_Interrupts(void) {
    __asm ("  CPSIE    I\n");
}
