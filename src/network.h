#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <w5500.h>

extern byte mac[];
extern uint8_t network_buffer[1500];
extern bool got_lldp;

extern Wiznet5500 w5500;

void network_init();
void network_generate_mac();

#endif
