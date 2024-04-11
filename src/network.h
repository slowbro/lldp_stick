#ifndef NETWORK_H
#define NETWORK_H

#include <w5500.h>

extern byte mac[];
extern uint8_t rbuf[1500];
extern bool got_lldp;

extern Wiznet5500 w5500;

void network_init();

#endif
