#include "sleep.h"
#include "config.h"
#include "display.h"
#include "button.h"
#include "ble.h"
#include "network.h"
#include "setting.h"
#include <Arduino_nRF5x_lowPower.h>

volatile bool interrupt = false;
uint32_t sleep_last_action = millis();

void intHandler(){
    interrupt = true;
}

void sleep_init_interrupts(){
    attachInterrupt(digitalPinToInterrupt(PIN_LBTN), intHandler, RISING);
    nRF5x_lowPower.enableWakeupByInterrupt(PIN_LBTN, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_RBTN), intHandler, RISING);
    nRF5x_lowPower.enableWakeupByInterrupt(PIN_RBTN, RISING);
}

void wake(){
    // wake up the peripherals
    //pinMode(PIN_PERIPH, OUTPUT);
    //digitalWrite(PIN_PERIPH, LOW);
    delay(200);
}

void sleep(){
    // disconnect BLE and shutdown the radio
    ble_end();

    // save settings if necessary
    if(setting_needs_save)
        setting_save();

    // end and reset the wizchip
    w5500.end();
    digitalWrite(PIN_WIZ_RESET, LOW);

    // clear the display and reset the SD1306
    display.clearDisplay();
    display.display();
    digitalWrite(PIN_OLED_RESET, LOW);

    // disable power to peripherals
    //digitalWrite(PIN_PERIPH, HIGH);

    // delay to stop any button press keeping the device awake
    // when sleeping manually (i.e. from the menu)
    delay(250);

    // power off the MCU
    nRF5x_lowPower.powerMode(POWER_MODE_OFF);
}

void keep_awake(){
    sleep_last_action = millis();
}

void sleep_autosleep(){
    if(settings.autosleep == 0)
        return;

    if((millis() - sleep_last_action) > (settings.autosleep * 1000)){
        uint32_t last_animation = 0;
        uint8_t animation_pos = 0;
        char animation[] = "Z ... z ... z ...";
        while(true){
            button_read();

            if(button_lbtn_pressed() || button_rbtn_pressed()){
                break;
            }
            
            if(millis() - last_animation > 500){
                display.clearDisplay();
                display.setCursor(14, 16);
                animation_pos++;

                if(animation_pos > 17)
                    sleep();

                char *disp = (char *)malloc(animation_pos+1);
                memcpy(disp, animation, animation_pos);
                disp[animation_pos] = '\0';
                display.println(disp);
                free(disp);
                display.display();

                last_animation = millis();
            }
        }
    }
}
