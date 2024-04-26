#include "menu.h"
#include "sleep.h"
#include "setting.h"
#include "battery.h"
#include "util.h"
#include "network.h"
#include "config.h"

bool menu_active = false;

oled_menu menu(&display, 4, 21);

menu_item* menu_main[] = {
    new menu_item_header(),
    new menu_item_display("BLE Setup", menu_ble_setup, menu_deselect, NULL),
    new menu_item_display("Device Info", menu_device_info, menu_deselect, NULL),
    new menu_item_submenu("Settings", 6, menu_settings),
    new menu_item_command("Poweroff", sleep),
    new menu_item_command("Back", menu_toggle),
    new menu_item_footer("< Next       Select >")
};

menu_item* menu_settings[] = {
    new menu_item_header(),
    new menu_item_display("MAC Address", menu_settings_mac_address, menu_settings_mac_address_lbtn, menu_settings_mac_address_rbtn),
    new menu_item_display("Autosleep Timer", menu_settings_autosleep, menu_settings_autosleep_lbtn, menu_deselect),
    new menu_item_display("Text Scroll Speed", menu_settings_scroll_speed, menu_settings_scroll_speed_lbtn, menu_deselect),
    new menu_item_back("Save Settings", menu_settings_save),
    new menu_item_footer("< Next       Select >")
};

void menu_init(){
    menu.set_menu(7, menu_main);
}

void menu_toggle(){
    menu_active = !menu_active;
    menu.reset();
}

void menu_deselect(){
    menu.deselect();
}

void menu_ble_setup(){
    display.println("Nothing here yet..");
    display.setCursor(0,24);
    display.println("< Back");
}

void menu_device_info(){
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
}

void menu_settings_mac_address(){
    display.setCursor(14, 6);
    
    char *macstr = mac_to_char(settings.mac_address);
    display.println(macstr);
    free(macstr);
    
    display.setCursor(0, 24);
    display.println("< Generate     Save >");
}

void menu_settings_mac_address_lbtn(){
    network_generate_mac(settings.mac_address);
    setting_needs_save = true;
}

void menu_settings_mac_address_rbtn(){
    w5500.change_mac_address(settings.mac_address);
    menu.deselect();
}

void menu_settings_autosleep(){
    display.setCursor(0,0);
    display.println("Autosleep Seconds:");
    display.setCursor(50, 10);
    display.println(settings.autosleep);
    display.setCursor(0, 24);
    display.println("< Next         Save >");
}

void menu_settings_autosleep_lbtn(){
    if(settings.autosleep < 300)
        settings.autosleep += 30;
    else
        settings.autosleep = 0;

    setting_needs_save = true;
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

void menu_settings_scroll_speed_lbtn(){
    if(settings.text_scroll_multiplier == 0.5){
        settings.text_scroll_multiplier = 2.0;
    } else if(settings.text_scroll_multiplier == 1.0){
        settings.text_scroll_multiplier = 0.5;
    } else {
        settings.text_scroll_multiplier = 1.0;
    }

    setting_needs_save = true;
}

void menu_settings_save(){
    setting_needs_save = true;
}
