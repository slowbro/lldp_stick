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
#include "setting.h"

void setup(){
    // perform wake/boot actions
    wake();

    // initialize/load settings (before BLE activation)
    setting_init();

    // set up the display
    display_init();

    // set up the buttons
    button_init();

    // initialize the wizchip
    network_init();

    // attach interrupts to both buttons to wake the mcu
    sleep_init_interrupts();

    // read the initial battery voltage
    battery_init();

    // BLE
    ble_init();

    // initialize menu classes
    menu_init();
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
        menu.update();

        if(button_lbtn_pressed()){
            menu.lbtn();
        }

        if(button_rbtn_pressed()){
            menu.rbtn();
        }
    } else {
        if(w5500.wizphy_getphylink() == 0){
            got_lldp = false;
            display_clear_buffer();
            display_set_buffer_line_centered(1, "* No Link *");
        } else {
            keep_awake();
            if(!got_lldp) {
                display_clear_buffer();
                display_set_buffer_line(1, "Waiting for LLDP..");
            }

            uint16_t len = w5500.readFrame(network_buffer, sizeof(network_buffer));
            // Ethertype 0x88CC
            if(len > 12 && network_buffer[12] == 136 && network_buffer[13] == 204){
                got_lldp = true;
                PDUInfo pinfo;
                lldp_process(network_buffer, len, &pinfo);

                display_clear_buffer();
                display_set_buffer_line(0, "Switch: ", pinfo.data, pinfo.SystemNameStart, pinfo.SystemNameLength);
                display_set_buffer_line(1, "Port: ", pinfo.data, pinfo.PortIdStart, pinfo.PortIdLength);
                display_set_buffer_line_int(2, "TTL: ", pinfo.data, pinfo.TTLStart, pinfo.TTLLength);
                // optional TLVs
                // this works for now but is kind of undefined behavior
                uint8_t line = 3;
                if(pinfo.PortDescriptionLength != 0){
                    display_set_buffer_line(line, "Desc: ", pinfo.data, pinfo.PortDescriptionStart, pinfo.PortDescriptionLength);
                    line++;
                }
                if(pinfo.SystemNameLength != 0){
                    display_set_buffer_line(line, "SysName: ", pinfo.data, pinfo.SystemNameStart, pinfo.SystemNameLength);
                    line++;
                }
                if(pinfo.SystemDescriptionLength != 0){
                    display_set_buffer_line(line, "SysDesc: ", pinfo.data, pinfo.SystemDescriptionStart, pinfo.SystemDescriptionLength);
                    line++;
                }
                if(pinfo.ManagementAddressLength != 0){
                    display_set_buffer_line(line, "Mgmt: ", pinfo.data, pinfo.ManagementAddressStart, pinfo.ManagementAddressLength);
                    line++;
                }
            }
        }
        display_print();

        if(button_lbtn_pressed()){
            display_buffer_advance();
        }

        if(button_rbtn_pressed()){
            menu_toggle();
        }
    }
}

