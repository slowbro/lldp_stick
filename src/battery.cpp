#include <Arduino.h>
#include "battery.h"
#include "config.h"

int battery_reading = 0;

void battery_read(){
    battery_reading = analogRead(VDIV);
}

float battery_voltage(){
    return battery_reading * BATTERY_COEFFICIENT;
}
