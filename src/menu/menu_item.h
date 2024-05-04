#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include "menu_constants.h"
#include "oled_menu.h"

class oled_menu;

class menu_item {
    protected:
        const char* text = NULL;
        byte type = MENU_ITEM_NONE;
        oled_menu* menu;
    public:
        menu_item(const char* text) : text(text) {}
        menu_item(const char* text, byte type) : text(text), type(type) {}

        void set_menu(oled_menu* menu);
        byte get_type(){ return type; }
        void set_text(const char* text){ this->text = text; };
        const char* get_text(){ return text; }

        virtual fptr get_callback(){ return NULL; };
        virtual void set_callBack(fptr callback){};
        virtual menu_item** get_submenu() { return NULL; }
        virtual menu_item* operator[](const uint8_t index) { return NULL; };
        virtual fptr get_display(){ return NULL; }
        virtual fptr get_lbtn(){ return NULL; }
        virtual fptr get_rbtn(){ return NULL; }
        virtual uint8_t get_size(){ return 0; }
};

class menu_item_header : public menu_item {
    protected:
        menu_item** parent = NULL;

        menu_item_header(const char* text, menu_item** parent, byte type)
            : menu_item(text, type), parent(parent) {}

    public:
        /**
         */
        menu_item_header() : menu_item_header(NULL, NULL, MENU_ITEM_MAIN_MENU_HEADER) {}
        /**
         * @param parent the parent menu item
         */
        menu_item_header(menu_item** parent)
            : menu_item_header(NULL, parent, MENU_ITEM_SUB_MENU_HEADER) {}

        menu_item** get_submenu() override { return this->parent; };

        menu_item* operator[](const uint8_t index) override { return parent[index]; }
};

class menu_item_footer : public menu_item {
    public:
        menu_item_footer() : menu_item(NULL, MENU_ITEM_END_OF_MENU) {}
        menu_item_footer(const char* key) : menu_item(key, MENU_ITEM_END_OF_MENU) {}
};

#endif
