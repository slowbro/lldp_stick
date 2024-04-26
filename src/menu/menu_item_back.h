#ifndef MENU_ITEM_BACK_H
#define MENU_ITEM_BACK_H

#include "oled_menu.h"
#include "menu_item_command.h"

class menu_item_back : public menu_item_command {
    public:
        menu_item_back(const char* key) : menu_item_command(key, NULL, MENU_ITEM_BACK) {}
        menu_item_back(const char* key, fptr callback) : menu_item_command(key, callback, MENU_ITEM_BACK) {}
};

#endif
