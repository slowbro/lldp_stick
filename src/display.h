#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdint.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

extern Adafruit_SSD1306 display;

void display_clear_buffer();
void display_set_buffer_line(int line, const char *str);
void display_set_buffer_line_centered(int line, const char *str);
void display_set_buffer_line(int line, uint8_t *data, uint8_t start, uint16_t len);
void display_set_buffer_line(int line, const char *prefix, uint8_t *data, uint8_t start, uint16_t len);
void display_print();
void display_set_header();
void display_set_footer();
void display_init();
void display_buffer_advance();
uint8_t display_buffer_length();

#endif
