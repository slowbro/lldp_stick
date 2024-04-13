#ifndef LLDP_H
#define LLDP_H

#include <stdint.h>
#include <Arduino.h>

struct PDUInfo {

    // the packet
    uint8_t data[1500];

    // TLV 1: Chassis ID
    uint8_t ChassisIdStart;
    uint8_t ChassisIdSubtype;
    uint16_t ChassisIdLength;

    // TLV 2: Port ID
    uint8_t PortIdStart;
    uint8_t PortIdSubtype;
    uint16_t PortIdLength;

    // TLV 3: TTL
    uint8_t TTLStart;
    uint8_t TTLLength;

    // TLV 4: Port Description
    uint8_t PortDescriptionStart;
    uint16_t PortDescriptionLength;

    // TLV 5: System Name
    uint8_t SystemNameStart;
    uint16_t SystemNameLength;

    // TLV 6: System Description
    uint8_t SystemDescriptionStart;
    uint16_t SystemDescriptionLength;

    // TLV 7: System Capabilities
    uint8_t SystemCapabilitiesStart;
    uint16_t SystemCapabilitiesLength;

    // TLV 8: Management Address
    uint8_t ManagementAddressStart;
    uint16_t ManagementAddressLength;

};

struct TLV_Org {

    // TLV 127
    uint8_t derp;

};

void process_lldp(uint8_t *data, uint16_t len, PDUInfo *pinfo);

#endif
