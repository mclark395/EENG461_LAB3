#include "sw1_int.h"
#include "stdint.h"
#include "common/tm4c123gh6pm.h"
#include "setup.h"
#include "timers.h"

#define MIN_CLOCKS_DEBOUNCE (int32_t)((int32_t)CYCLES_PER_SEC/100)


void PORTF_int_handler(void){
	GPIO_PORTF_IM_R &= ~SW1_PIN; //Disable interrupt
	GPIO_PORTF_ICR_R |= SW1_PIN; //Clear interrupt

	
	static int32_t uptime_last = 0;
	static int32_t cycles_last = 0;
	
	__asm("CPSID    I\n");
	int32_t uptime_temp = uptime;
	int32_t cycles_temp = CYCLES_PER_SEC-TIMER0_TAR_R;
	__asm("CPSIE    I\n");
	
	int32_t cycles_passed = ((uptime_temp-uptime_last)*CYCLES_PER_SEC) + (cycles_temp-cycles_last);

	if(cycles_passed > MIN_CLOCKS_DEBOUNCE) {
		uptime_last = uptime_temp;
		cycles_last = cycles_temp;
		GPIO_PORTF_DATA_R ^= GREEN_LED;
	}

	GPIO_PORTF_IM_R |= SW1_PIN; //Re-enable interrupt
}

