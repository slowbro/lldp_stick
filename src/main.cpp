#include <Arduino.h>

#include "config.h"
#include "button.h"
#include "lldp.h"
#include "display.h"
#include "menu.h"
#include "sleep.h"
#include "network.h"
#include "battery.h"
#include "ble.h"

void setup(){
    pinMode(LBTN, INPUT_PULLDOWN);
    pinMode(RBTN, INPUT_PULLDOWN);
    pinMode(VDIV, INPUT);
    //pinMode(PERIPH, OUTPUT);

    // perform wake/boot actions
    wake();

    // set up the display
    display_init();

    // initialize the wizchip
    network_init();

    // attach interrupts to both buttons to wake the mcu
    sleep_init_interrupts();

    // read the initial battery voltage
    battery_read();

    // BLE
    ble_init();
}

void loop(){
    // autosleep
    sleep_autosleep();

    // do BLE actions
    ble_poll();

    // read the battery voltage
    battery_read();

    // manage buttons
    button_read();

    // display things
    if(menu_active){
        menu_display();

        if(button_lbtn_pressed()){
            menu_lbtn();
        }

        if(button_rbtn_pressed()){
            menu_rbtn();
        }
    } else {
        if(w5500.wizphy_getphylink() == 0){
            got_lldp = false;
            display_clear_buffer();
            display_set_buffer_line_centered(0, "* No Link *");
        } else {
            keep_awake();
            if(!got_lldp) {
                display_clear_buffer();
                display_set_buffer_line(0, "Waiting for LLDP..");
            }

            uint16_t len = w5500.readFrame(network_buffer, sizeof(network_buffer));
            // Ethertype 0x88CC
            if(len > 12 && network_buffer[12] == 136 && network_buffer[13] == 204){
                got_lldp = true;
                PDUInfo pinfo;
                process_lldp(network_buffer, len, &pinfo);
                display_clear_buffer();
                display_set_buffer_line(0, "Switch: ", pinfo.data, pinfo.SystemNameStart, pinfo.SystemNameLength);
                display_set_buffer_line(1, "Port: ", pinfo.data, pinfo.PortIdStart, pinfo.PortIdLength);
                display_set_buffer_line(2, "TTL: ", pinfo.data, pinfo.TTLStart, pinfo.TTLLength);
                display_set_buffer_line(3, "Desc: ", pinfo.data, pinfo.PortDescriptionStart, pinfo.PortDescriptionLength);
                display_set_buffer_line(4, "SysName: ", pinfo.data, pinfo.SystemNameStart, pinfo.SystemNameLength);
                display_set_buffer_line(5, "SysDesc: ", pinfo.data, pinfo.SystemDescriptionStart, pinfo.SystemDescriptionLength);
            }
        }
        display_print();

        if(button_lbtn_pressed()){
            display_buffer_advance();
        }

        if(button_rbtn_pressed()){
            menu_active = true;
        }
    }
}

