#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <stdint.h>
#include "sleep.h"
#include "config.h"

void button_init();
void button_read();
bool button_lbtn_pressed();
bool button_rbtn_pressed();

#endif
