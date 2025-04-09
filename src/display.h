#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdint.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "battery.h"
#include "ble.h"
#include "setting.h"
#include "config.h"

extern Adafruit_SSD1306 display;

void display_clear_buffer();
void display_set_buffer_line(uint8_t, const char *);
void display_set_buffer_line_centered(uint8_t, const char *);
void display_set_buffer_line(uint8_t, uint8_t *, uint16_t, uint16_t);
void display_set_buffer_line(uint8_t, const char *, uint8_t *, uint16_t, uint16_t);
void display_set_buffer_line_int(uint8_t, const char *, uint8_t *, uint16_t, uint16_t);
void display_print();
void display_set_header();
void display_set_footer();
void display_init();
void display_buffer_advance();
uint8_t display_buffer_length();

#endif
