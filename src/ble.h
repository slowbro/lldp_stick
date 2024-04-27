#ifndef BLE_H
#define BLE_H

#include <BLEPeripheral.h>

extern bool ble_connected;
extern const char *ble_id;

void ble_init();
void ble_begin();
void ble_end();
void ble_poll();
void ble_peripheral_connect_handler(BLECentral&);
void ble_peripheral_disconnect_handler(BLECentral&);
void ble_mac_updated_handler(BLECentral&, BLECharacteristic&);
void ble_characteristic_update_battery(float);
void ble_characteristic_update_mac(byte *);
uint32_t ble_radio_notification_init(uint32_t, uint8_t, uint8_t);
void SWI3_IRQHandler(bool);

#endif
