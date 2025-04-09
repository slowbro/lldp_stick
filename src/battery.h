#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include "ble.h"
#include "config.h"

extern int battery_reading;

void battery_init();
void battery_read();
float battery_voltage();

#endif
