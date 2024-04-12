#ifndef SLEEP_H
#define SLEEP_H

#include <Arduino_nRF5x_lowPower.h>

extern volatile bool interrupt;
void intHandler();
void sleep_init_interrupts();
void sleep();
void wake();
void keep_awake();
void sleep_autosleep();

#endif
