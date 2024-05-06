#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

char *mac_to_char(byte[]);
byte *char_to_mac(char *);

#endif
