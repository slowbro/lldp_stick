#include "battery.h"

int battery_reading = 0;
uint32_t battery_last_read = 0;

void battery_init(){
    pinMode(PIN_VDIV, INPUT);

    // force a read of the battery
    battery_reading = analogRead(PIN_VDIV);
}

void battery_read(){
    if(millis() - battery_last_read > BATTERY_READ_DELAY){
        battery_reading = analogRead(PIN_VDIV);
        battery_last_read = millis();
        ble_characteristic_update_battery(battery_voltage());
    }
}

float battery_voltage(){
    return battery_reading * BATTERY_COEFFICIENT;
}
