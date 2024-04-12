#include "ble.h"
#include <BLEPeripheral.h>

BLEPeripheral blePeripheral = BLEPeripheral();
BLEService bleService = BLEService("19b10000e8f2537e4f6cd104768a1214");
bool ble_connected = false;

void ble_init(){
    blePeripheral.setLocalName("LLDP Stick");
    blePeripheral.setAdvertisedServiceUuid(bleService.uuid());
    blePeripheral.addAttribute(bleService);
    blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
    blePeripheral.begin();
}

void blePeripheralConnectHandler(BLECentral& central) {
    // central connected event handler
    ble_connected = true;
}

void blePeripheralDisconnectHandler(BLECentral& central) {
    // central disconnected event handler
    ble_connected = false;
}

