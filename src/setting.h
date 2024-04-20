#ifndef SETTING_H
#define SETTING_H

#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>

#define SETTING_VERSION 2

struct Settings {
    // the mac address of the device
    byte mac_address[6];

    //the amount of time (seconds) to idle before sleep
    uint8_t autosleep;
};

extern Settings settings;
extern bool setting_needs_save;

void setting_init();
void setting_save();

#endif
