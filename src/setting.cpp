#include "setting.h"
#include <EEPROM.h>
#include "network.h"
#include "ble.h"
#include "config.h"

Settings settings;
bool setting_needs_save = false;

void setting_init(){
    uint8_t version = EEPROM.read(0);
    if(version != SETTING_VERSION){
        // version is out of sync, overwrite the settings
        network_generate_mac(settings.mac_address);

        // set default settings
        settings.autosleep = SLEEP_AUTOSLEEP_SECONDS;
        settings.text_scroll_multiplier = 1.0;
        settings.ble_keep_awake = false;
        settings.oled_dim = false;

        // saving here is *generally* a safe activity,
        // since this function is called before BLE init
        setting_save();

        // write the settings version to the first byte of EEPROM
        EEPROM.write(0, SETTING_VERSION);
    } else {
        // version is in sync, populate 'settings' struct
        EEPROM.get(1, settings);
    }
}

void setting_save(){
    EEPROM.put(1, settings);
    setting_needs_save = false;
}

void setting_safe_save(){
    ble_end();
    setting_save();
    ble_begin();
}
