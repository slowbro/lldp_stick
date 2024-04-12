#include "ble.h"
#include "network.h"
#include "util.h"
#include <BLEPeripheral.h>

BLEPeripheral                ble_peripheral = BLEPeripheral();
BLEService                   ble_service = BLEService("19b10000e8f2537e4f6cd104768a1214");
BLEFixedLengthCharacteristic ble_mac_characteristic = BLEFixedLengthCharacteristic("fff1", BLERead | BLEWrite, 17);
bool ble_connected = false;

void ble_init(){
    ble_peripheral.setLocalName("LLDP Stick");
    ble_peripheral.setAdvertisedServiceUuid(ble_service.uuid());

    ble_peripheral.addAttribute(ble_service);
    ble_peripheral.addAttribute(ble_mac_characteristic);

    ble_peripheral.setEventHandler(BLEConnected, ble_peripheral_connect_handler);
    ble_peripheral.setEventHandler(BLEDisconnected, ble_peripheral_disconnect_handler);

    char *macstr = mac_to_char(mac);
    ble_mac_characteristic.setValue(macstr);
    free(macstr);

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

