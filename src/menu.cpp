#include "menu.h"
#include "sleep.h"
#include "config.h"

bool menu_active = false;
uint8_t menu_item_selected = 0;
int8_t menu_item_top = 0;
const char *menu_items[] = {
    "BLE Setup\0",
    "Menu Item 2\0",
    "Poweroff\0",
    "Back\0"
};

void displayMenu(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    for(int i = menu_item_top, o=0; i<MENU_ITEMS; i++,o++){
        if(o > OLED_MAX_LINES-2)
            break;

        const char *prefix;
        char *line;
        if(i == menu_item_selected){
            prefix = "> ";
        } else {
            prefix = "  ";
        }
        int len = strlen(menu_items[i]);

        line = (char *)malloc(len+3);
        memcpy(line, prefix, 2);
        memcpy(line+2, menu_items[i], len);
        line[len+2] = '\0';

        display.println(line);

        free(line);
    }

    display.println("< Next       Select >\0");
    display.display();
}

void menuLbtn(){
    menu_item_selected++;
    if(menu_item_selected >= MENU_ITEMS){
        menu_item_selected = 0;
    }

    menu_item_top = menu_item_selected-2;
    if(menu_item_top < 0){
        menu_item_top = 0;
    }
}

void menuRbtn(){
    // rudimentary item handling until i can get callbacks or something working
    switch(menu_item_selected){
        case 0: // BLE Setup
        case 1: // Menu Item 2
            break;
        case 2: //Poweroff
            sleep();
            break;
        case 3: //Back
            menu_active = false;    
            menu_item_selected = 0;
            menu_item_top = 0;
        break;
    }
}
