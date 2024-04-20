#ifndef MENU_H
#define MENU_H

#include "stdint.h"
#include "display.h"

#define MENU_ITEMS 5

extern bool menu_active;
extern uint8_t menu_item_selected;
extern int8_t menu_item_top;

void menu_display();
void menu_lbtn();
void menu_rbtn();
void menu_device_info();
void menu_ble_setup();

#endif
