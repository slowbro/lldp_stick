#include "network.h"
#include "config.h"

byte mac[] = { 0xae, 0x03, 0xf3, 0xc7, 0x08, 0x78 };
uint8_t rbuf[1500];
bool got_lldp = false;

Wiznet5500 w5500(CS);

void network_init(){
    digitalWrite(WIZ_RESET, HIGH);
    // give the wiznet a bit to initialize
    delay(500);
    w5500.begin(mac);
}

