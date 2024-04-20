#ifndef SETTING_H
#define SETTING_H

#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>

#define SETTING_VERSION 2

struct Setting {
    // the mac address of the device
    byte mac_address[6];

    //the amount of time (seconds) to idle before sleep
    uint8_t autosleep;
};

extern Setting settings;

void setting_init();

#endif
