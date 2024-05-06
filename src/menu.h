#ifndef MENU_H
#define MENU_H

#include "stdint.h"
#include "display.h"
#include "menu/oled_menu.h"
#include "menu/menu_item_command.h"
#include "menu/menu_item_display.h"
#include "menu/menu_item_submenu.h"
#include "menu/menu_item_back.h"

extern bool menu_active;

void menu_init();
void menu_toggle();
void menu_deselect();
void menu_print_footer(const char *);
void menu_ble_info();
void menu_device_info();
void menu_settings_mac_address();
void menu_settings_mac_address_lbtn();
void menu_settings_mac_address_rbtn();
void menu_settings_autosleep();
void menu_settings_autosleep_lbtn();
void menu_settings_scroll_speed();
void menu_settings_scroll_speed_lbtn();
void menu_settings_ble();
void menu_settings_ble_lbtn();
void menu_settings_ble_wake();
void menu_settings_ble_wake_lbtn();
void menu_settings_oled_dim();
void menu_settings_oled_dim_lbtn();
void menu_settings_save();

extern menu_item* menu_main[];
extern menu_item* menu_settings[];
extern oled_menu menu;

#endif
