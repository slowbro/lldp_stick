#include "sleep.h"
#include "config.h"
#include "display.h"
#include <Arduino_nRF5x_lowPower.h>

volatile bool interrupt = false;

void intHandler(){
    interrupt = true;
}

void sleep_init_interrupts(){
    attachInterrupt(digitalPinToInterrupt(LBTN), intHandler, RISING);
    nRF5x_lowPower.enableWakeupByInterrupt(LBTN, RISING);
    attachInterrupt(digitalPinToInterrupt(RBTN), intHandler, RISING);
    nRF5x_lowPower.enableWakeupByInterrupt(RBTN, RISING);
}

void sleep(){
    digitalWrite(WIZ_RESET, LOW);
    display.clearDisplay();
    display.display();
    display.ssd1306_command(SSD1306_DISPLAYOFF);	// put the OLED display in sleep mode
    display.ssd1306_command(0x8D);					// disable charge pump
    display.ssd1306_command(0x10);					// disable charge pump
    //digitalWrite(OLED_RESET, LOW);
    delay(250);
    nRF5x_lowPower.powerMode(POWER_MODE_OFF);
}

