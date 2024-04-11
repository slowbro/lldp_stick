#include "menu.h"
#include "sleep.h"
#include "util.h"
#include "network.h"
#include "config.h"

bool menu_active = false;
uint8_t menu_item_selected = 0;
int8_t menu_item_entered = -1;
int8_t menu_item_top = 0;
const char *menu_items[] = {
    "BLE Setup\0",
    "Device Info\0",
    "Poweroff\0",
    "Back\0"
};

void displayMenu(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    if(menu_item_entered != -1){
        // display the menu item we're in
        switch(menu_item_entered){
            case 0:
                menuBleSetup();
                break;
            case 1:
                menuDeviceInfo();
                break;
            default:
                menu_item_entered = -1;
        }
    } else {
        // display the main menu
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

        display.println("< Next       Select >");
        display.display();
    }
}

void menuLbtn(){
    if(menu_item_entered != -1){
        // go 'back' from the entered menu item
        menu_item_entered = -1;
    } else {
        menu_item_selected++;
        if(menu_item_selected >= MENU_ITEMS){
            menu_item_selected = 0;
        }

        menu_item_top = menu_item_selected-2;
        if(menu_item_top < 0){
            menu_item_top = 0;
        }
    }
}

void menuRbtn(){
    if(menu_item_entered == -1){
        // rudimentary item handling until i can get callbacks or something working
        switch(menu_item_selected){
            case 0: // BLE Setup
                menu_item_entered = 0;
                menuBleSetup();
                break;
            case 1: // Device Info
                menu_item_entered = 1;
                menuDeviceInfo();
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
}

void menuDeviceInfo(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    //mac address
    display.print("Mac ");
    char *macstr = mac_to_char(mac);
    display.println(macstr);
    free(macstr);

    // battery voltage?
    display.println("Battery: ");

    display.println();
    display.println("< Back");

    display.display();
}

void menuBleSetup(){

}
