#include "menu.h"

const char *git_rev = STRINGIFY(GIT_REV);
bool menu_active = false;

oled_menu menu(&display, OLED_LINES, OLED_MAXLEN);

menu_item* menu_main[] = {
    new menu_item_header(),
    new menu_item_display("BLE Info", menu_ble_info, menu_deselect, NULL),
    new menu_item_display("Device Info", menu_device_info, menu_deselect, NULL),
    new menu_item_submenu("Games", 4, menu_games),
    new menu_item_submenu("Settings", 9, menu_settings),
    new menu_item_command("Poweroff", sleep),
    new menu_item_command("Back", menu_toggle),
    new menu_item_footer("< Next       Select >")
};

menu_item* menu_settings[] = {
    new menu_item_header(),
    new menu_item_display("MAC Address", menu_settings_mac_address, menu_settings_mac_address_lbtn, menu_settings_mac_address_rbtn),
    new menu_item_display("Autosleep Timer", menu_settings_autosleep, menu_settings_autosleep_lbtn, menu_deselect),
    new menu_item_display("Text Scroll Speed", menu_settings_scroll_speed, menu_settings_scroll_speed_lbtn, menu_deselect),
    new menu_item_display("BLE Enable/Disable", menu_settings_ble, menu_settings_ble_lbtn, menu_deselect),
    new menu_item_display("BLE Keep-Awake", menu_settings_ble_wake, menu_settings_ble_wake_lbtn, menu_deselect),
    new menu_item_display("Dim OLED", menu_settings_oled_dim, menu_settings_oled_dim_lbtn, menu_deselect),
    new menu_item_back("Save Settings", menu_settings_save),
    new menu_item_footer("< Next       Select >")
};

menu_item* menu_games[] = {
    new menu_item_header(),
    new menu_item_command("Snake", game_snake_run),
    new menu_item_back("Back", NULL),
    new menu_item_footer("< Next       Select >")
};

void menu_init(){
    menu.set_menu(8, menu_main);
}

void menu_toggle(){
    menu_active = !menu_active;
    menu.reset();
}

void menu_deselect(){
    menu.deselect();
}

void menu_print_footer(const char *left, const char *right){
    if(left != NULL){
        display.setCursor(0, 56);
        display.println(left);
    }

    if(right != NULL){
        display.setCursor(OLED_WIDTH-(strlen(right)*6), 56);
        display.println(right);
    }
}

void menu_ble_info(){
    display.println(ble_id);
    display.print("Status: ");
    if(ble_connected)
        display.println("Connected");
    else
        display.println("Disconnected");

    menu_print_footer("< Back", NULL);
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
    display.print("Firmware: ");
    display.println(git_rev);

    menu_print_footer("< Back", NULL);
}

void menu_settings_mac_address(){
    display.setCursor(14, 6);
    
    char *macstr = mac_to_char(settings.mac_address);
    display.println(macstr);
    free(macstr);
    
    menu_print_footer("< Generate", "Save >");
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
    if(settings.autosleep != 0){
        display.setCursor(50, 20);
        display.println(settings.autosleep);
    } else {
        display.setCursor(42, 20);
        display.println("Never");
    }

    menu_print_footer("< Next", "Save >");
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
    display.setCursor(40, 20);
    if(settings.text_scroll_multiplier == 2.0){
        display.println("Slow");
    } else if(settings.text_scroll_multiplier == 1.0){
        display.println("Normal");
    } else {
        display.println("Fast");
    }

    menu_print_footer("< Next", "Save >");
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

void menu_settings_ble(){
    display.println("Disable BLE?");

    display.setCursor(50, 20);
    if(settings.ble_disable)
        display.println("Yes");
    else
        display.println("No");

    menu_print_footer("< Toggle", "Save >");
}

void menu_settings_ble_lbtn(){
    settings.ble_disable = !settings.ble_disable;

    if(settings.ble_disable)
        ble_end();
    else
        ble_begin();
}

void menu_settings_ble_wake(){
    display.println("Stay Awake on BLE?");

    display.setCursor(50, 20);
    if(settings.ble_keep_awake)
        display.println("Yes");
    else
        display.println("No");

    menu_print_footer("< Toggle", "Save >");
}

void menu_settings_ble_wake_lbtn(){
    settings.ble_keep_awake = !settings.ble_keep_awake;
    setting_needs_save = true;
}

void menu_settings_oled_dim(){
    display.println("Dim OLED?");

    display.setCursor(50, 20);
    if(settings.oled_dim)
        display.println("Yes");
    else
        display.println("No");

    menu_print_footer("< Toggle", "Save >");
}

void menu_settings_oled_dim_lbtn(){
    settings.oled_dim = !settings.oled_dim;
    display.dim(settings.oled_dim);
    setting_needs_save = true;
}

void menu_settings_save(){
    setting_needs_save = true;
    
    if(settings.ble_disable)
        setting_save();
}
