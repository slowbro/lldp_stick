#include "sleep.h"
#include "display.h"
#include "button.h"
#include "ble.h"
#include "network.h"
#include "setting.h"
#include "config.h"
#include <Arduino_nRF5x_lowPower.h>

volatile bool interrupt = false;
uint32_t sleep_last_action = millis();

/**
 * Interrupt Handler function for button presses. Functionally meaningless.
 *
 */
void intHandler(){
    interrupt = true;
}

/**
 * Initialize button interrupts, to wake the MCU on button-press.
 *
 */
void sleep_init_interrupts(){
    attachInterrupt(digitalPinToInterrupt(PIN_LBTN), intHandler, RISING);
    nRF5x_lowPower.enableWakeupByInterrupt(PIN_LBTN, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_RBTN), intHandler, RISING);
    nRF5x_lowPower.enableWakeupByInterrupt(PIN_RBTN, RISING);
}

/**
 * Perform wake actions, such as enabling peripherals.
 *
 */
void wake(){
    // wake up the peripherals
    pinMode(PIN_PERIPH, OUTPUT);
    digitalWrite(PIN_PERIPH, LOW);
    delay(200);
}

/**
 * Sleep the device, first performing necessary actions.
 *
 */
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
    digitalWrite(PIN_PERIPH, HIGH);

    // delay to stop any button press keeping the device awake
    // when sleeping manually (i.e. from the menu)
    delay(250);

    // power off the MCU
    nRF5x_lowPower.powerMode(POWER_MODE_OFF);
}

/**
 * Keep the device awake by resetting the sleep_last_action variable to now.
 *
 */
void keep_awake(){
    sleep_last_action = millis();
}

/**
 * Auto-sleep the device after an interval (or not, if the interval == 0).
 *
 */
void sleep_autosleep(){
    // never sleep if autosleep interval is 0
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
