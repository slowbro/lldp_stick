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
    blePeripheral.poll();

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
            display_set_buffer_line(0, "* No Link *");
        } else {
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
                display_set_buffer_line(0, "Switch: ", network_buffer, pinfo.SystemNameStart, pinfo.SystemNameLength);
                display_set_buffer_line(1, "Port: ", network_buffer, pinfo.PortIdStart, pinfo.PortIdLength);
            }
        }
        display_print();

        if(button_rbtn_pressed()){
            menu_active = true;
        }
    }
}

