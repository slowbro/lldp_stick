#ifndef SLEEP_H
#define SLEEP_H

#include "display.h"
#include "button.h"
#include "ble.h"
#include "network.h"
#include "setting.h"
#include "config.h"
#include <Arduino_nRF5x_lowPower.h>

void intHandler();
void sleep_init_interrupts();
void sleep();
void wake();
void keep_awake();
void sleep_autosleep();

#endif
