#include "util.h"

// convert a given byte array of mac address octets to a char representation (de:ad:be:ef:ca:fe)
char *mac_to_char(byte mac[]){
    char *macstr = (char *)malloc(18);

    for(uint8_t i=0, o=0; i<6;i++,o+=3){
        char buf[3];
        sprintf(buf, "%02X", mac[i]);
        memcpy(&macstr[o], buf, 2);
        if(i != 5)
            macstr[o+2] = 58; // :
    }
    macstr[17] = '\0';

    return macstr;
}

// convert a given char representation (de:ad:be:ef:ca:fe) to a byte array
byte *char_to_mac(char *mac){
    unsigned int imac[6];
    byte *macbytes = (byte *)malloc(sizeof(byte)*6);

    sscanf(mac, "%x:%x:%x:%x:%x:%x", &imac[0], &imac[1], &imac[2], &imac[3], &imac[4], &imac[5]);
    for(uint8_t i=0;i<6;i++)
        macbytes[i] = (unsigned char)imac[i];

    return macbytes;
}
