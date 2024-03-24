#ifndef LLDP_H
#define LLDP_H

#include <stdint.h>
#include <Arduino.h>

struct PDUInfo {

  uint8_t ChassisIdStart;
  // TLV 1
  uint8_t ChassisIdSubtype;
  uint16_t ChassisIdLength;
  // TLV 2
  uint8_t PortIdStart;
  uint8_t PortIdSubtype;
  uint16_t PortIdLength;
  // TLV 4
  uint8_t PortDescriptionStart;
  uint16_t PortDescriptionLength;
  // TLV 5
  uint8_t SystemNameStart;
  uint16_t SystemNameLength;
  // TLV 6
  uint8_t SystemDescriptionStart;
  uint16_t SystemDescriptionLength;

};

struct TLV_Org {

  // TLV 127
  uint8_t derp;

};

bool processLLDP(uint8_t* data, uint16_t len, PDUInfo* pinfo);

#endif
