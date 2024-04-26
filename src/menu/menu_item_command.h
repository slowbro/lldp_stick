#ifndef MENU_ITEM_COMMAND_H
#define MENU_ITEM_COMMAND_H

#include "menu_constants.h"

class menu_item_command : public menu_item {
    private:
        fptr callback;

    protected:
        menu_item_command(const char* key, fptr callback, uint8_t type)
            : menu_item(key, type) {
            this->callback = callback;
        }

    public:
        menu_item_command(const char* key, fptr callback)
            : menu_item(key, MENU_ITEM_COMMAND) {
            this->callback = callback;
        }

        fptr get_callback() override { return callback; };
        void set_callBack(fptr callback) override { this->callback = callback; };
};

#endif
