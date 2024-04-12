#ifndef BLE_H
#define BLE_H

#include <BLEPeripheral.h>

extern BLEPeripheral blePeripheral;
extern BLEService bleService;
extern bool ble_connected;

void ble_init();
void blePeripheralConnectHandler(BLECentral& central);
void blePeripheralDisconnectHandler(BLECentral& central);

#endif
