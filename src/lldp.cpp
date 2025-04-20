#include "lldp.h"

/**
 * Process LLDP TLVs from +data+ of +len+ length. Store into +pinfo+.
 *
 */
void lldp_process(uint8_t *data, uint16_t len, PDUInfo *pinfo) {
    uint8_t tlv_type;
    uint16_t tlv_len = 0;

    // copy the packet into pinfo
    memcpy(pinfo->data, data, len);

    // set all the lengths to 0 so we know which TLVs were received
    lldp_set_defaults(pinfo);

    // starting at byte 14, start discovering TLVs and their length
    for (uint16_t pos = 14; pos < len;) {
        // extract the headers for a new TLV
        uint16_t tlv = ((uint16_t)data[pos] << 8) + data[pos+1];

        // first 7 bits are the TLV ID
        tlv_type = tlv >> 9;

        // last 9 bits are the TLV length
        tlv_len = tlv & 0b0000000111111111;

        //skip the type-length header altogether, putting 'pos' at the start of the value
        pos+=2;

        // store the TLV info in pinfo (PDUInfo struct)
        switch (tlv_type) {
            case 0:
                // END OF PDU
                // skip to the end to terminate the loop
                pos = len;
                break;
            case 1:
                // Chassis ID has a special bit for subtype
                pinfo->ChassisIdSubtype = data[pos];
                // start is +1 because of the subtype
                pinfo->ChassisIdStart = pos + 1;
                // likewise, length is -1 due to the subtype being taken into account in the length
                pinfo->ChassisIdLength = tlv_len - 1;
                break;
            case 2:
                // Port ID has a special bit for subtype
                pinfo->PortIdSubtype = data[pos];
                // start is +1 because of the subtype
                pinfo->PortIdStart = pos + 1;
                // likewise, length is -1 due to the subtype being taken into account in the length
                pinfo->PortIdLength = tlv_len - 1;
                break;
            case 3:
                pinfo->TTLStart = pos;
                pinfo->TTLLength = tlv_len;
                break;
            case 4:
                pinfo->PortDescriptionStart = pos;
                pinfo->PortDescriptionLength = tlv_len;
                break;
            case 5:
                pinfo->SystemNameStart = pos;
                pinfo->SystemNameLength = tlv_len;
                break;
            case 6:
                pinfo->SystemDescriptionStart = pos;
                pinfo->SystemDescriptionLength = tlv_len;
                break;
            case 7:
                pinfo->SystemCapabilitiesStart = pos;
                pinfo->SystemCapabilitiesLength = tlv_len;
                break;
            case 8:
                pinfo->ManagementAddressStart = pos;
                pinfo->ManagementAddressLength = tlv_len;
                break;
        }
        pos += tlv_len;
    }
}

/**
 * Set the defaults for all TLV lengths so we can tell which TLVs we have received.
 *
 */
void lldp_set_defaults(PDUInfo *pinfo){
    pinfo->ChassisIdLength = 0;
    pinfo->PortIdLength = 0;
    pinfo->TTLLength = 0;
    pinfo->PortDescriptionLength = 0;
    pinfo->SystemNameLength = 0;
    pinfo->SystemDescriptionLength = 0;
    pinfo->SystemCapabilitiesLength = 0;
    pinfo->ManagementAddressLength = 0;
}
