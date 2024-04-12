#include <Arduino.h>

#include "config.h"
#include "button.h"
#include "lldp.h"
#include "display.h"
#include "menu.h"
#include "sleep.h"
#include "network.h"
#include "battery.h"

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
}

void loop(){
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
            clearBuffer();
            setBufferLine(0, "* No Link *");
        } else {
            if(!got_lldp) {
                clearBuffer();
                setBufferLine(0, "Waiting for LLDP..");
            }

            uint16_t len = w5500.readFrame(rbuf, sizeof(rbuf));
            // Ethertype 0x88CC
            if(len > 12 && rbuf[12] == 136 && rbuf[13] == 204){
                got_lldp = true;
                PDUInfo pinfo;
                processLLDP(rbuf, len, &pinfo);
                clearBuffer();
                setBufferLine(0, "Switch: ", rbuf, pinfo.SystemNameStart, pinfo.SystemNameLength);
                setBufferLine(1, "Port: ", rbuf, pinfo.PortIdStart, pinfo.PortIdLength);
            }
        }
        printDisplay();

        if(button_rbtn_pressed()){
            menu_active = true;
        }
    }
}
