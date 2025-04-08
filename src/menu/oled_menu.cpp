#include "oled_menu.h"
#include <Arduino.h>

void oled_menu::draw_menu(){
    display->clearDisplay();
    display->setTextSize(1);
    display->setCursor(0, 0);

    if(selected_item == -1){
        // draw the menu items if nothing is selected
        for(uint8_t i = top; i <= top+max_lines-1; i++){
            menu_item *item = menu_table[i];

            // skip nonexistant items (i.e. item count < max_lines)
            if(item == NULL)
                continue;
                
            // end processing when we get to the end of the menu
            if(item->get_type() == MENU_ITEM_END_OF_MENU)
                break;

            const char *text = item->get_text();
            if(text != NULL){
                const char *prefix;
                char *line;
                if(i == cursor_position){
                    prefix = "> ";
                } else {
                    prefix = "  ";
                }
                int len = strlen(text);

                line = (char *)malloc(len+3);
                memcpy(line, prefix, 2);
                memcpy(line+2, text, len);
                line[len+2] = '\0';
                display->println(line);
                free(line);
            }
        }

        // if the footer has text, show it
        menu_item *footer = menu_table[menu_table_size-1];
        if(footer->get_text() != NULL){
            display->setCursor(0, 8*max_lines);
            display->print(footer->get_text());
        }
    } else {
        // run the action for the 'selected' item
        menu_item *item = menu_table[selected_item];
        switch(item->get_type()){
            case MENU_ITEM_DISPLAY:
                // display the screen as defined by the menu_item
                (item->get_display())();
                break;
            case MENU_ITEM_SUBMENU:
                last_menu_table = menu_table;
                menu_table = item->get_submenu();
                last_menu_table_size = menu_table_size;
                menu_table_size = item->get_size();
                reset();
                break;
            case MENU_ITEM_BACK:
                this->deselect();
                break;
            default:
                selected_item = -1;
        }
    }

    display->display();
}

void oled_menu::set_menu(uint8_t count, menu_item** menu){
    this->menu_table_size = count;
    this->menu_table = menu;

    // if the footer has text, decrement max_lines
    if(this->menu_table[count-1]->get_text() != NULL)
        max_lines--;

    // add a reference to the this oled_menu class to menu_item(s)
    for(uint8_t i = 0; i < count; i++){
        this->menu_table[i]->set_menu(this);

        // traverse the submenus too
        if(this->menu_table[i]->get_type() == MENU_ITEM_SUBMENU){
            menu_item** items = this->menu_table[i]->get_submenu();
            for(uint8_t o = 0; o < this->menu_table[i]->get_size(); o++)
                items[o]->set_menu(this);
        }   
    }
}

void oled_menu::lbtn(){
    if(selected_item == -1){
        cursor_position++;
        if(cursor_position > max_lines){
            top++;
        }

        // when we try to go past the past the last menu item,
        // wrap around to the top
        if(cursor_position+2 > menu_table_size){
            top = 1;
            cursor_position = 1;
        }
    } else {
        menu_item *item = this->menu_table[selected_item];
        if(item->get_lbtn() != NULL)
            (item->get_lbtn())();
    }
}

void oled_menu::rbtn(){
    if(selected_item == -1){
        menu_item *item = this->menu_table[cursor_position];
        if(item->get_type() != MENU_ITEM_NONE)
            selected_item = cursor_position;

        if(item->get_callback() != NULL)
            (item->get_callback())();

    } else {
        menu_item *item = this->menu_table[selected_item];
        if(item->get_rbtn() != NULL)
            (item->get_rbtn())();
    }
}

void oled_menu::deselect(){
    menu_item *item = this->menu_table[selected_item];
    if(last_menu_table != NULL && item->get_type() == MENU_ITEM_BACK){
        // going "back" from a submenu
        menu_table = last_menu_table;
        menu_table_size = last_menu_table_size;
        last_menu_table = NULL;
        last_menu_table_size = 0;
        reset();
    } else {
        selected_item = -1;
    }
}
