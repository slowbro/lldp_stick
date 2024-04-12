#include "button.h"
#include "sleep.h"
#include "config.h"
#include <stdint.h>
#include <Arduino.h>

int lbtn_reading, rbtn_reading, lbtn_last, rbtn_last;
uint32_t last_debounce_lbtn, last_debounce_rbtn = 0;
int lbtn, rbtn = 0;

void button_read(){
    lbtn_reading = digitalRead(LBTN);
    rbtn_reading = digitalRead(RBTN);

    if(lbtn_last != lbtn){
        last_debounce_lbtn = millis();
    }

    if(rbtn_last != rbtn){
        last_debounce_rbtn = millis();
    }

    if((millis() - last_debounce_lbtn) > BUTTON_DEBOUNCE_DELAY){
        if(lbtn_reading != lbtn){
            lbtn = lbtn_reading;
        }
    }

    if((millis() - last_debounce_rbtn) > BUTTON_DEBOUNCE_DELAY){
        if(rbtn_reading != rbtn){
            rbtn = rbtn_reading;
        }
    }

    lbtn_last = lbtn_reading;
    rbtn_last = rbtn_reading;
}

bool button_lbtn_pressed(){
    if(lbtn == HIGH){
        lbtn = LOW;
        keep_awake();
        return true;
    }

    return false;
}

bool button_rbtn_pressed(){
    if(rbtn == HIGH){
        rbtn = LOW;
        keep_awake();
        return true;
    }

    return false;
}
