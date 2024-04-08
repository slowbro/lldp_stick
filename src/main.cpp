#include <Arduino.h>
#include <w5500.h>

#include "config.h"
#include "lldp.h"
#include "display.h"
#include "menu.h"
#include "sleep.h"

byte mac[] = { 0xae, 0x03, 0xf3, 0xc7, 0x08, 0x78 };
uint8_t rbuf[1500];
bool got_lldp = false;
int lbtn, rbtn, lbtn_reading, rbtn_reading, lbtn_last, rbtn_last;
uint32_t last_debounce_lbtn, last_debounce_rbtn = 0;

Adafruit_SSD1306 display(128, SSD1306_LCDHEIGHT, &Wire, OLED_RESET);
Wiznet5500 w5500(CS);

void read_buttons(){
    lbtn_reading = digitalRead(LBTN);
    rbtn_reading = digitalRead(RBTN);

    if(lbtn_last != lbtn){
        last_debounce_lbtn = millis();
    }

    if(rbtn_last != rbtn){
        last_debounce_rbtn = millis();
    }

    if((millis() - last_debounce_lbtn) > 50){
        if(lbtn_reading != lbtn){
            lbtn = lbtn_reading;
        }
    }

    if((millis() - last_debounce_rbtn) > 50){
        if(rbtn_reading != rbtn){
            rbtn = rbtn_reading;
        }
    }

    lbtn_last = lbtn_reading;
    rbtn_last = rbtn_reading;
}

void setup(){
    pinMode(WIZ_RESET, OUTPUT);
    pinMode(OLED_RESET, OUTPUT);
    pinMode(LBTN, INPUT_PULLDOWN);
    pinMode(RBTN, INPUT_PULLDOWN);

    // set up the display
    display.begin(0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(32,16);
    display.setTextColor(WHITE);
    display.print(F("Booting..."));
    display.display();

    digitalWrite(WIZ_RESET, HIGH);
    // give the wiznet a bit to initialize
    delay(500);
    w5500.begin(mac);

    // attach interrupts to both buttons to wake the mcu
    sleep_init_interrupts();
}

void loop(){
    // manage buttons
    read_buttons();

    // display things
    if(menu_active){
        displayMenu();

        if(lbtn == HIGH){
            menuLbtn();
            lbtn = LOW;
        }

        if(rbtn == HIGH){
            menuRbtn();
            rbtn = LOW;
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

        if(rbtn == HIGH){
            menu_active = true;
            rbtn = LOW;
        }
    }
}
