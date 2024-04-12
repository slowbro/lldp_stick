#include <Arduino.h>
#include "battery.h"
#include "config.h"

int battery_reading = 0;
uint32_t battery_last_read = 0;

void battery_read(){
    if(millis() - battery_last_read > BATTERY_READ_DELAY){
        battery_reading = analogRead(VDIV);
        battery_last_read = millis();
    }
}

float battery_voltage(){
    return battery_reading * BATTERY_COEFFICIENT;
}
