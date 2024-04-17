#include "ble.h"
#include "network.h"
#include "battery.h"
#include "util.h"
#include <BLEPeripheral.h>

BLEPeripheral                ble_peripheral = BLEPeripheral();

BLEService                   ble_mac_service        = BLEService("fff0");
BLEFixedLengthCharacteristic ble_mac_characteristic = BLEFixedLengthCharacteristic("fff1", BLERead | BLEWrite, 17);
BLEDescriptor                ble_mac_descriptor     = BLEDescriptor("2901", "MAC Address");

BLEService                   ble_battery_service        = BLEService("ddd0");
BLEFloatCharacteristic       ble_battery_characteristic = BLEFloatCharacteristic("ddd1", BLERead);
BLEDescriptor                ble_battery_descriptor     = BLEDescriptor("2901", "Battery Voltage");

bool ble_connected = false;

void ble_init(){
    ble_peripheral.setLocalName("LLDP Stick");

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

    char *macstr = mac_to_char(mac);
    ble_mac_characteristic.setValue(macstr);
    free(macstr);

    ble_battery_characteristic.setValue(battery_voltage());

    ble_peripheral.begin();
}

void ble_poll(){
    ble_peripheral.poll();
}

void ble_peripheral_connect_handler(BLECentral& central) {
    // central connected event handler
    ble_connected = true;
}

void ble_peripheral_disconnect_handler(BLECentral& central) {
    // central disconnected event handler
    ble_connected = false;
}

void ble_characteristic_update_battery(float voltage){
    ble_battery_characteristic.setValue(voltage);
}

void ble_characteristic_update_mac(byte *mac){
    char *macstr = mac_to_char(mac);
    ble_mac_characteristic.setValue(macstr);
    free(macstr);
}
