#ifndef SETTING_H
#define SETTING_H

#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>

#define SETTING_VERSION 1

struct Setting {
    byte mac_address[6];
};

extern Setting settings;

void setting_init();

#endif
