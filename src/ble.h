#ifndef BLE_H
#define BLE_H

#include <BLEPeripheral.h>

extern bool ble_connected;

void ble_init();
void ble_poll();
void ble_peripheral_connect_handler(BLECentral& central);
void ble_peripheral_disconnect_handler(BLECentral& central);
void ble_characteristic_update_battery(float);

#endif
