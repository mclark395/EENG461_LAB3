//
// Created by Mark Clark on 10/3/23.
//

#ifndef EENG461_LAB_2_TIMERS_H
#define EENG461_LAB_2_TIMERS_H

extern volatile int sec_count;

#define CYCLES_PER_SEC 16000000;

void configureTimer (void);
void timerISR (void);

#endif //EENG461_LAB_2_TIMERS_H
