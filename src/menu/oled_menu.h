/**
 * Heavily inspired by/adapted/straight-up stolen from: https://github.com/forntoh/LcdMenu
 */
#ifndef OLED_MENU_H
#define OLED_MENU_H

#include "Adafruit_SSD1306.h"
#include "stdint.h"
#include <Arduino.h>
#include "menu_item.h"

class menu_item;

class oled_menu {
    private:
        Adafruit_SSD1306* display;
        uint8_t rows, cols;
        bool enable_update = true;
        uint8_t menu_table_size;
        uint8_t last_menu_table_size;
        menu_item** menu_table;
        menu_item** last_menu_table = NULL;
        uint8_t cursor_position = 1;
        uint8_t top = 1;
        uint8_t max_lines = 0;
        int16_t selected_item = -1;

        void draw_menu();

    public:
        oled_menu(Adafruit_SSD1306* display, uint8_t rows, uint8_t cols){
            this->display = display;
            this->rows = rows;
            this->cols = cols;
            this->max_lines = rows;
        }

        void set_menu(uint8_t count, menu_item** menu);

        void update(){
            if(!enable_update)
                return;
            draw_menu();
        }

        void show(){
            this->enable_update = true;
        }

        void hide(){
            this->enable_update = false;
        }

        void toggle(){
            this->enable_update = !this->enable_update;
        }

        void reset(){
            cursor_position = 1;
            top = 1;
            selected_item = -1;
        }

        void lbtn();
        void rbtn();
        void deselect();
};

#endif
