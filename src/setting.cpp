#include "setting.h"
#include <EEPROM.h>
#include "ble.h"
#include "network.h"

Setting settings;

void setting_init(){
    uint8_t version = EEPROM.read(0);
    if(version != SETTING_VERSION){
        // version is out of sync, overwrite the settings
        network_generate_mac();
        memcpy(&settings.mac_address, &network_mac, sizeof(network_mac));

        EEPROM.put(1, &settings);

        // write the settings version to the first byte of EEPROM
        EEPROM.write(0, SETTING_VERSION);
    } else {
        // version is in sync, populate 'settings' struct
        EEPROM.get(1, settings);
    }
}

