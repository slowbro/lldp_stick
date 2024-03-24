#include <Arduino.h>
#include <w5500.h>

#include "config.h"
#include "lldp.h"
#include "display.h"

byte mac[] = { 0xae, 0x03, 0xf3, 0xc7, 0x08, 0x78 };
byte rbuf[1500];

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
    display.setTextSize(2);
    display.setCursor(6,OLED_VERTICAL_OFFSET);
    display.setTextColor(WHITE);
    display.println(F("LLDPStick!"));
    display.display();

    Wire.begin();

    digitalWrite(WIZ_RESET, HIGH);

    w5500.begin(mac);

    delay(1000);

    display.clearDisplay();
    display.setTextSize(1);
    display.println(F("Waiting for LLDP.."));
    display.display();
}

void loop(){
    uint16_t len = w5500.readFrame(rbuf, sizeof(rbuf));
    if ( len > 12 && rbuf[12] == 136 && rbuf[13] == 204 ) {
        PDUInfo pinfo;
        processLLDP(rbuf, len, &pinfo);
        setBufferLine(0, rbuf, pinfo.SystemNameStart, pinfo.SystemNameLength);
        setBufferLine(1, rbuf, pinfo.PortIdStart, pinfo.PortIdLength);
        printDisplay();
    }

}
