#include "common/tm4c123gh6pm.h"
#include "setup.h"
#include <stdint.h>

void setup(void) {
	// Enable GPIO clock
	SYSCTL_RCGCGPIO_R |= (1 << 5);
	
	// Configure pins
	GPIO_PORTF_DEN_R |= RGB_PINS;
	GPIO_PORTF_DR8R_R |= RGB_PINS;
	
	// Set initial values
	GPIO_PORTF_DATA_R &= ~RGB_PINS; //All off to start
	
	// Set pin directions
	GPIO_PORTF_DIR_R |= RGB_PINS;
	
	//SW2 pullup
	GPIO_PORTF_PUR_R |= SW1_PIN;
	GPIO_PORTF_DEN_R |= SW1_PIN;
	
	//Enable interrupts on value of buttons
	GPIO_PORTF_IEV_R &= ~SW1_PIN; //Falling edge
	GPIO_PORTF_IM_R |= SW1_PIN;
	NVIC_EN0_R |= (1 << 30); // Enable Port F interrupts in nvic
}
