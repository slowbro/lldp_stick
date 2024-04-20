#include "network.h"
#include "setting.h"
#include "config.h"

byte network_mac[6];
uint8_t network_buffer[1500];
bool got_lldp = false;

Wiznet5500 w5500(PIN_WIZ_CS);

void network_init(){
    pinMode(PIN_WIZ_RESET, OUTPUT);
    digitalWrite(PIN_WIZ_RESET, HIGH);
    // give the wiznet a bit to initialize
    delay(500);
    w5500.begin(settings.mac_address);
}

void network_generate_mac(){
    randomSeed(analogRead(PIN_VDIV));
    for(uint8_t i=0;i<6;i++){
        network_mac[i] = random(0, 255);
    }
}
