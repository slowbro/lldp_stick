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

void wake(){
    // wake up the peripherals
    //digitalWrite(PERIPH, LOW);
    delay(200);
}

void sleep(){
    digitalWrite(WIZ_RESET, LOW);
    display.clearDisplay();
    display.display();
    digitalWrite(OLED_RESET, LOW);
    //digitalWrite(PERIPH, HIGH);
    delay(250);
    nRF5x_lowPower.powerMode(POWER_MODE_OFF);
}

