#ifndef SETTING_H
#define SETTING_H

#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "network.h"
#include "ble.h"
#include "config.h"

#define SETTING_VERSION 7

struct Settings {
    // the mac address of the device
    byte mac_address[6];

    //the amount of time (seconds) to idle before sleep
    uint16_t autosleep;

    // the multiplier we use to determine text scrolling speed
    float text_scroll_multiplier;

    // keep awake when BLE connected?
    bool ble_keep_awake;

    // dim the oled?
    bool oled_dim;

    // disable BLE?
    bool ble_disable;
};

extern Settings settings;
extern bool setting_needs_save;

void setting_init();
void setting_save();
void setting_safe_save();

#endif
