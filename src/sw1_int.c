#include "common/tm4c123gh6pm.h"
#include "setup.h"

#define MIN_CLOCKS_DEBOUNCE 16000 //~1ms


void PORTF_int_handler(void){
	GPIO_PORTF_IM_R &= ~SW1_PIN; //Disable interrupt
	GPIO_PORTF_ICR_R &= ~SW1_PIN; //Clear interrupt
	
	static int uptime_last = 0;
	static int cycles_last = 0;
	
	//We might re-use these values - this prevents redundant reads.
	//We also only care about the cycles passed since we entered the ISR, and don't need to read fresh values.
	asm("CPSID    I");
	int uptime_temp = uptime;
	int cycles_temp = TIMER0_TAV_R;
	asm("CPSIE    I");
	
	int cycles_passed = (uptime_temp-uptime_last)*CYCLES_PER_SEC + cycles_temp;
	
	if(cycles_passed > MIN_CLOCKS_DEBOUNCE) {
		uptime_last = uptime_temp;
		cycles_last = cycles_temp;
		GPIO_PORTF_DATA_R ^= GREEN_LED;
	}
	//reenable interrupt
	GPIO_PORTF_IM_R |= SW1_PIN;
}
//CPU Clock cycles used in ISR:
// 2 (Disable int)
// 4 (load uptime_last, cycles_last)
// 8 (Atomic load of uptime and timer's value)
// 3 (Subtract and multiply-accumulate)
// 4 (branch + pipeline flush)
// 2 (enable interrupt)

// Extra, if we're taking action:
// 4 (Storing "last" values)
// 5 (Load, xor, store)

//So 23 or 32 cycles, excluding context switching time. ~ 2 microseconds @ 16mhz
