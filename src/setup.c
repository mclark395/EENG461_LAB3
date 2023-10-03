#include "common/tm4c123gh6pm.h"
#include "setup.h"

#define GREEN_LED (1 << 3)
#define BLUE_LED (1 << 2)
#define RED_LED (1 << 1)

void setup(void) {
	// Enable GPIO clock
	SYSCTL_RCGCGPIO_R |= (1 << 5);
	
	// Configure pins
	GPIO_PORTF_DEN_R |= RGB_PINS;
	GPIO_PORTF_DR8R_R |= RGB_PINS;
	
	// Set initial values
	// Set pin directions
	GPIO_PORTF_DIR_R |= RGB_PINS;
	
}
