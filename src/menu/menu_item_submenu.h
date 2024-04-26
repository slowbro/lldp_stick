#ifndef MENU_ITEM_SUBMENU_H
#define MENU_ITEM_SUBMENU_H

#include "menu_constants.h"

class menu_item_submenu : public menu_item_header {
    protected:
        uint8_t size;
    public:
        menu_item_submenu(const char* text, uint8_t size, menu_item** parent)
            : menu_item_header(text, parent, MENU_ITEM_SUBMENU) {
            this->size = size;
        }

        uint8_t get_size(){
            return size;
        }
};

#endif
