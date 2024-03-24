#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdint.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

extern Adafruit_SSD1306 display;

void setBufferLine(int line, char *str);
void setBufferLine(int line, uint8_t *data, uint8_t start, uint16_t len);
void printDisplay(uint8_t size);
void printDisplay();

#endif
