#include <stdbool.h>
#include <stdint.h>

#include "sw1_int.h"

#include "common/tm4c123gh6pm.h"

#include "setup.h"
#include "timers.h"


#define MIN_CLOCKS_DEBOUNCE (int32_t)((int32_t)CYCLES_PER_SEC/100)

#define TIMER_ISR_IS_PENDING (TIMER0_MIS_R & TIMER_ICR_TATOCINT)

void PORTF_int_handler(void){
	// ASSUMPTION: ONLY SW1 ON PORTF is toggled. If more pins were being toggled, then we would need to dispatch a different handeler for each.
	// Such code might look like this.
	
	if(GPIO_PORTF_MIS_R & SW1_PIN) {
		GPIO_PORTF_IM_R &= ~SW1_PIN; //Disable interrupt
		GPIO_PORTF_ICR_R |= SW1_PIN; //Clear interrupt
		sw1_debounce();
		GPIO_PORTF_IM_R |= SW1_PIN; //Re-enable interrupt
	}
}

void sw1_debounce(void){
	// static = persist between calls to function, is this context.
	static int32_t uptime_last = 0; 
	static int32_t cycles_last = 0;
	static enum { RELEASED, PRESSED } button_state = RELEASED;
	
	// Since we are triggering on both rising and falling edges, we need to determine which it was.
	const enum { FALLING, RISING } edge_type = (GPIO_PORTF_DATA_BITS_R[SW1_PIN] == SW1_PIN);
	
	int32_t uptime_temp;
	int32_t cycles_temp;
	
	// Early exit for don't-care state combinations
	// Recall, button pressed = falling edge
	if(	((button_state == PRESSED)  && (edge_type == FALLING )) ||
		((button_state == RELEASED) && (edge_type == RISING)) ) {
		return;
	}
	
	// Atomic read of what time it is. 
	// There is an edge case if TIMER0A has overflowed since we entered the ISR.
	// There is an edge case if TIMER0A has overflowed after we check if it has, and before we read the timer value
	__asm("CPSID    I\n");
	do {
		if(TIMER_ISR_IS_PENDING) timerISR();
		
		uptime_temp = uptime_seconds;
		cycles_temp = CYCLES_PER_SEC-TIMER0_TAR_R;
		
	// If the counter overflowed during this code block, then our reads of uptime and cycles are invalid. Re-do them.
	} while (TIMER_ISR_IS_PENDING); 
	__asm("CPSIE    I\n");
	
	int32_t seconds_passed = (uptime_temp - uptime_last);
	// Edge case: If too many seconds had passed then the next calculation would overflow. 
	// After 2 full seconds, enough time has passed, so just clamp this instead of using larger integers.
	if(seconds_passed > 2) seconds_passed = 2; 
	
	//This is the one, true, correct calculation of how long, in cycles, it has been.
	int32_t cycles_passed  = (cycles_temp - cycles_last) + seconds_passed*CYCLES_PER_SEC;

	if(cycles_passed > MIN_CLOCKS_DEBOUNCE) {
		uptime_last = uptime_temp;
		cycles_last = cycles_temp;
		
		//Recall that certain combinations of button states already resulted in early-exit.
		//If made it here then the only possibilities are (PRESSED & RISING) or (RELEASED & FALLING)
		switch(button_state) {
			case PRESSED:
				button_state = RELEASED;
				break;
			
			case RELEASED:
				button_state = PRESSED;
				GPIO_PORTF_DATA_BITS_R[GREEN_LED] ^= GREEN_LED;
		}
	}
}
