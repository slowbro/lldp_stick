#ifndef SLEEP_H
#define SLEEP_H

#include <Arduino_nRF5x_lowPower.h>

extern volatile bool interrupt;
void intHandler();
void sleep_init_interrupts();
void sleep();
void wake();

#endif
