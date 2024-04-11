#ifndef UTIL_H
#define UTIL_H

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

#endif
