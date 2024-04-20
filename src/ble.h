#ifndef BLE_H
#define BLE_H

#include <BLEPeripheral.h>

extern bool ble_connected;

void ble_init();
void ble_begin();
void ble_end();
void ble_poll();
void ble_peripheral_connect_handler(BLECentral& central);
void ble_peripheral_disconnect_handler(BLECentral& central);
void ble_mac_updated_handler(BLECentral& central, BLECharacteristic& characteristic);
void ble_characteristic_update_battery(float);
void ble_characteristic_update_mac(byte *mac);

#endif
