#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdint.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

extern Adafruit_SSD1306 display;
extern unsigned long last_animation;

void clearBuffer();
void setBufferLine(int line, char *str);
void setBufferLine(int line, uint8_t *data, uint8_t start, uint16_t len);
void setBufferLine(int line, char *prefix, uint8_t *data, uint8_t start, uint16_t len);
void printDisplay(uint8_t size);
void printDisplay();
void setHeader();
void setFooter();
void display_init();

#endif
