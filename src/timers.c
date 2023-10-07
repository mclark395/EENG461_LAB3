//
// Created by Mark Clark on 10/3/23.
//

#include <stdint.h>
#include "timers.h"
#include "main.h"
#include "setup.h"
#include "common/tm4c123gh6pm.h"

volatile int32_t uptime_seconds;

void configureTimer (void) {

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0; //Enable Run Mode Clock Gating Control for Timer 0

    while (!(SYSCTL_PRTIMER_R & SYSCTL_RCGCTIMER_R0)) {}

    TIMER0_CTL_R &= ~TIMER_CTL_TAEN; //Disable Timer
    TIMER0_CFG_R = TIMER_CFG_32_BIT_TIMER;
    TIMER0_TAMR_R |= TIMER_TAMR_TAMR_PERIOD; //Set Timer to count down periodically
    TIMER0_TAILR_R = CYCLES_PER_SEC - 1;
    TIMER0_TAPR_R = 0;
    TIMER0_ICR_R |= TIMER_ICR_TATOCINT; //Clear Interrupt
    TIMER0_IMR_R |= TIMER_IMR_TATOIM; //Enable Interrupt as Timeout
    NVIC_EN0_R = 1 << (INT_TIMER0A - 16);
    TIMER0_CTL_R |= TIMER_CTL_TAEN; //Enable Timer

}

void timerISR (void) {

    TIMER0_IMR_R &= ~TIMER_IMR_TATOIM; //Disable Interrupt
    TIMER0_ICR_R |= TIMER_ICR_TATOCINT; //Clear Interrupt

    if (sec_count < 4) {
        sec_count++; //Increment second counter
    } else {
        sec_count = 0;
    }
	uptime_seconds++;
	
    TIMER0_IMR_R |= TIMER_IMR_TATOIM; //Enable Interrupt
}
