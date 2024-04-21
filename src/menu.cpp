#include "menu.h"
#include "sleep.h"
#include "util.h"
#include "network.h"
#include "battery.h"
#include "setting.h"
#include "config.h"

bool menu_active = false;
uint8_t menu_item_selected = 0, menu_settings_item_selected = 0;
int8_t menu_item_entered = -1, menu_settings_item_entered = -1;
int8_t menu_item_top = 0, menu_settings_item_top = 0;
const char *menu_items[] = {
    "BLE Setup\0",
    "Device Info\0",
    "Settings\0",
    "Poweroff\0",
    "Back\0"
};
const char *menu_settings_items[] = {
    "MAC Address\0",
    "Autosleep Timer\0",
    "Text Scroll Speed\0",
    "Save Settings\0",
    "Back\0"
};

void menu_display(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    if(menu_item_entered != -1){
        // display the menu item we're in
        switch(menu_item_entered){
            case 0:
                menu_ble_setup();
                break;
            case 1:
                menu_device_info();
                break;
            case 2:
                menu_settings();
                break;
            default:
                menu_item_entered = -1;
        }
    } else {
        // display the main menu
        for(int i = menu_item_top, o=0; i<MENU_ITEMS; i++,o++){
            if(o > OLED_LINES-2)
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

void menu_lbtn(){
    if(menu_item_entered == -1){
        // scroll through main menu items
        menu_item_selected++;
        if(menu_item_selected >= MENU_ITEMS){
            menu_item_selected = 0;
        }

        menu_item_top = menu_item_selected-2;
        if(menu_item_top < 0){
            menu_item_top = 0;
        }
    } else if(menu_item_entered == 2){
        // settings menu
        switch(menu_settings_item_entered){
            case -1:
                // scroll through settings menu items
                menu_settings_item_selected++;
                if(menu_settings_item_selected >= MENU_SETTINGS_ITEMS){
                    menu_settings_item_selected = 0;
                }

                menu_settings_item_top = menu_settings_item_selected-2;
                if(menu_settings_item_top < 0){
                    menu_settings_item_top = 0;
                }
                break;
            case 0:
                {
                    // generate a new mac address
                    byte *mac = network_generate_mac();
                    memcpy(&settings.mac_address, mac, sizeof(settings.mac_address));
                    free(mac);
                    break;
                }
            case 1:
                // increment/rollover the autosleep timer
                if(settings.autosleep < 300)
                    settings.autosleep += 30;
                else
                    settings.autosleep = 0;
                break;
            case 2:
                if(settings.text_scroll_multiplier == 0.5){
                    settings.text_scroll_multiplier = 2.0;
                } else if(settings.text_scroll_multiplier == 1.0){
                    settings.text_scroll_multiplier = 0.5;
                } else {
                    settings.text_scroll_multiplier = 1.0;
                }
                break;
        }
    } else {
        // go 'back' from the entered menu item
        menu_item_entered = -1;
    }
}

void menu_rbtn(){
    if(menu_item_entered == -1){
        // main menu rbtn actions
        switch(menu_item_selected){
            case 0: // BLE Setup
                menu_item_entered = 0;
                break;
            case 1: // Device Info
                menu_item_entered = 1;
                break;
            case 2: // Settings
                menu_item_entered = 2;
                break;
            case 3: //Poweroff
                sleep();
                break;
            case 4: //Back
                menu_active = false;
                menu_item_selected = 0;
                menu_item_top = 0;
                break;
        }
    } else if(menu_item_entered == 2){
        // settings menu rbtn actions
        if(menu_settings_item_entered == -1){
            switch(menu_settings_item_selected){
                case 0: // MAC Address
                    menu_settings_item_entered = 0;
                    break;
                case 1: // Autosleep Timer
                    menu_settings_item_entered = 1;
                    break;
                case 2: // Text Scroll Speed
                    menu_settings_item_entered = 2;
                    break;
                case 3: // Save Settings
                    setting_needs_save = true;
                case 4: //Back
                    menu_settings_item_top = 0;
                    menu_settings_item_selected = 0;
                    menu_item_entered = -1;
                    menu_settings_item_entered = -1;
                    break;
            }
        } else {
            switch(menu_settings_item_entered){
                case 0:
                    // save mac address
                    w5500.change_mac_address(settings.mac_address);
                    setting_needs_save = true;
                    menu_settings_item_entered = -1;
                    break;
                case 1:
                    // "save" autosleep timer - really just go back
                    setting_needs_save = true;
                    menu_settings_item_entered = -1;
                    break;
                case 2:
                    // "save" scroll speed modifier - really just go back
                    setting_needs_save = true;
                    menu_settings_item_entered = -1;
                    break;
            }
        }
    }
}

void menu_device_info(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    //mac address
    display.print("Mac ");
    char *macstr = mac_to_char(w5500._mac_address);
    display.println(macstr);
    free(macstr);

    // battery voltage
    display.print("Battery: ");
    display.print(battery_voltage(), 2);
    display.println("v");

    display.println();
    display.println("< Back");

    display.display();
}

void menu_ble_setup(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.println("Nothing here yet..");
    display.setCursor(0,24);
    display.println("< Back");
    display.display();
}

void menu_settings(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    if(menu_settings_item_entered != -1){
        switch(menu_settings_item_entered){
            case 0: // Mac Address
                menu_settings_mac_address();
                break;
            case 1: // Autosleep Timer
                menu_settings_autosleep();
                break;
            case 2: // Scroll Speed
                menu_settings_scroll_speed();
                break;
            default:
                menu_settings_item_entered = -1;
        }
    } else {
        // display the main menu
        for(int i = menu_settings_item_top, o=0; i<MENU_SETTINGS_ITEMS; i++,o++){
            if(o > OLED_LINES-2)
                break;

            const char *prefix;
            char *line;
            if(i == menu_settings_item_selected){
                prefix = "> ";
            } else {
                prefix = "  ";
            }
            int len = strlen(menu_settings_items[i]);

            line = (char *)malloc(len+3);
            memcpy(line, prefix, 2);
            memcpy(line+2, menu_settings_items[i], len);
            line[len+2] = '\0';

            display.println(line);

            free(line);
        }

        display.println("< Next       Select >");
    }

    display.display();
}

void menu_settings_mac_address(){
    display.setCursor(14, 6);
    
    char *macstr = mac_to_char(settings.mac_address);
    display.println(macstr);
    free(macstr);
    
    display.setCursor(0, 24);
    display.println("< Generate     Save >");
}

void menu_settings_autosleep(){
    display.setCursor(0,0);
    display.println("Autosleep Seconds:");
    display.setCursor(50, 10);
    display.println(settings.autosleep);
    display.setCursor(0, 24);
    display.println("< Next         Save >");
}

void menu_settings_scroll_speed(){
    display.setCursor(0,0);
    display.println("Text Scroll Speed:");
    display.setCursor(40, 10);
    if(settings.text_scroll_multiplier == 2.0){
        display.println("Slow");
    } else if(settings.text_scroll_multiplier == 1.0){
        display.println("Normal");
    } else {
        display.println("Fast");
    }
    display.setCursor(0, 24);
    display.println("< Next         Save >");
}
