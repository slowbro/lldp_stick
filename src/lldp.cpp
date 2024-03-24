#include "lldp.h"

bool processLLDP(uint8_t *data, uint16_t len, PDUInfo *pinfo) {
  uint8_t tlv_type;
  uint16_t tlv_len = 0;

  // starting at byte 14, start discovering TLVs and their length
  for (uint16_t pos = 14; pos < len; pos++) {
    // extract the headers for a new TLV
    uint16_t tlv = ((uint16_t)data[pos] << 8) + data[++pos];
    // first 7 bits are the TLV ID
    tlv_type = tlv >> 9;
    // last 9 bits are the TLV length
    tlv_len =  tlv & 0b0000000111111111;

    /*Serial.print(F("\nFound tlv "));
    Serial.print(tlv_type, DEC);
    Serial.print(F(" ("));
    Serial.print(tlv);
    Serial.print(F(") len "));
    Serial.println(tlv_len);*/

    // store the TLV info in pinfo (PDUInfo struct)
    switch (tlv_type) {
      case 0:
        // END OF PDU
        // skip to the end to terminate the loop
        pos = len;
        break;
      case 1:
        pinfo->ChassisIdSubtype = data[pos + 1];
        pinfo->ChassisIdStart = pos + 2;
        pinfo->ChassisIdLength = tlv_len - 1;
        break;
      case 2:
        pinfo->PortIdSubtype = data[pos + 1];
        pinfo->PortIdStart = pos + 2;
        pinfo->PortIdLength = tlv_len - 1;
        break;
      case 5:
        pinfo->SystemNameStart = pos + 1;
        pinfo->SystemNameLength = tlv_len;
    }
    pos += tlv_len;
  }
  return true;
}

