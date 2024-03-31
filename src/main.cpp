#include <Arduino.h>
#include <w5500.h>

#include "config.h"
#include "lldp.h"
#include "display.h"

byte mac[] = { 0xae, 0x03, 0xf3, 0xc7, 0x08, 0x78 };
uint8_t rbuf[1500];
bool got_lldp = false;

Adafruit_SSD1306 display(128, SSD1306_LCDHEIGHT, &Wire, OLED_RESET);
Wiznet5500 w5500(CS);

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

    Wire.begin();

    digitalWrite(WIZ_RESET, HIGH);

    w5500.begin(mac);

    delay(1000);

}

void loop(){
    if(w5500.wizphy_getphylink() == 0){
        got_lldp = false;
        clearBuffer();
        setBufferLine(0, "* No Link *");
    } else {
        if(!got_lldp) {
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
}
