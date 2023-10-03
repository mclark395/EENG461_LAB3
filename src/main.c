//
// Created by Mark Clark on 10/3/23.
//

#include "main.h"
#include "setup.h"
#include <stdint.h>
#include <driver/common/tm4c123gh6pm.h>

int sec_count = 0;

int main (void) {
    /*
     * Blue is PF2
     * Pin: 7 6 5 4 3 2 1 0
     * Bit: 0 0 0 0 0 1 0 0
     * Val: 0x04
     */
    GPIO_PORTF_DATA_R |= BLUE_LED;


    return (0);
}

