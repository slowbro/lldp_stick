#include "ble.h"

// set the BLE identifier at compile-time
const char *ble_id = "LLDP Stick " STRINGIFY(BLE_ID);
bool ble_connected = false;

// usually in 'app_util_platform.h' but apparently we have to make it ourselves
// for use with radio notification
nrf_nvic_state_t nrf_nvic_state;

// THE peripheral
BLEPeripheral                ble_peripheral = BLEPeripheral();

// MAC Address
BLEService                   ble_mac_service        = BLEService("fff0");
BLEFixedLengthCharacteristic ble_mac_characteristic = BLEFixedLengthCharacteristic("fff1", BLERead | BLEWrite, 17);
BLEDescriptor                ble_mac_descriptor     = BLEDescriptor("2901", "MAC Address");

//Battery Voltage
BLEService                   ble_battery_service        = BLEService("ddd0");
BLEFloatCharacteristic       ble_battery_characteristic = BLEFloatCharacteristic("ddd1", BLERead);
BLEDescriptor                ble_battery_descriptor     = BLEDescriptor("2901", "Battery Voltage");

void ble_init(){
    ble_peripheral.setLocalName(ble_id);

    ble_peripheral.setAdvertisedServiceUuid(ble_mac_service.uuid());
    ble_peripheral.addAttribute(ble_mac_service);
    ble_peripheral.addAttribute(ble_mac_characteristic);
    ble_peripheral.addAttribute(ble_mac_descriptor);

    ble_peripheral.setAdvertisedServiceUuid(ble_battery_service.uuid());
    ble_peripheral.addAttribute(ble_battery_service);
    ble_peripheral.addAttribute(ble_battery_characteristic);
    ble_peripheral.addAttribute(ble_battery_descriptor);

    ble_peripheral.setEventHandler(BLEConnected, ble_peripheral_connect_handler);
    ble_peripheral.setEventHandler(BLEDisconnected, ble_peripheral_disconnect_handler);
    ble_mac_characteristic.setEventHandler(BLEWritten, ble_mac_updated_handler);

    char *macstr = mac_to_char(settings.mac_address);
    ble_mac_characteristic.setValue(macstr);
    free(macstr);

    ble_battery_characteristic.setValue(battery_voltage());

    // enable a software interrupt for when the radio is inactive
    ble_radio_notification_init(3, NRF_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE, NRF_RADIO_NOTIFICATION_DISTANCE_800US);

    if(!settings.ble_disable)
        ble_peripheral.begin();
}

void ble_begin(){
    ble_peripheral.begin();
}

void ble_end(){
    ble_peripheral.end();
}

void ble_poll(){
    if(settings.ble_disable)
        return;

    ble_peripheral.poll();

    if(settings.ble_keep_awake && ble_connected)
        keep_awake();
}

void ble_peripheral_connect_handler(BLECentral& central) {
    // central connected event handler
    ble_connected = true;
}

void ble_peripheral_disconnect_handler(BLECentral& central) {
    // central disconnected event handler
    ble_connected = false;
}

void ble_mac_updated_handler(BLECentral& central, BLECharacteristic& characteristic){
    char newmac[17];
    memset(newmac, 0, 17);
    memcpy(newmac, ble_mac_characteristic.value(), ble_mac_characteristic.valueLength());

    byte *newmacbytes = char_to_mac(newmac);
    memcpy(&settings.mac_address, newmacbytes, sizeof(settings.mac_address));
    free(newmacbytes);

    w5500.change_mac_address(settings.mac_address);
}

void ble_characteristic_update_battery(float voltage){
    ble_battery_characteristic.setValue(voltage);
}

void ble_characteristic_update_mac(byte *mac){
    char *macstr = mac_to_char(mac);
    ble_mac_characteristic.setValue(macstr);
    free(macstr);
}

uint32_t ble_radio_notification_init(uint32_t irq_priority, uint8_t notification_type, uint8_t notification_distance){
    uint32_t err_code;

    err_code = sd_nvic_ClearPendingIRQ(SWI1_IRQn);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = sd_nvic_SetPriority(SWI1_IRQn, irq_priority);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = sd_nvic_EnableIRQ(SWI1_IRQn);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Configure the event
    return sd_radio_notification_cfg_set(notification_type, notification_distance);
}

void SWI1_IRQHandler(bool radio_evt){
    if(setting_needs_save){
        setting_save();
    }
}
