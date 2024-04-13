#include "config.h"
#include "display.h"
#include "battery.h"
#include "ble.h"

char *display_buffer[8];
int display_line_start[8] = {0};
int display_buffer_size[8] = {0};
uint8_t display_buffer_top = 0;
uint32_t display_last_animation, display_last_header_animation = 0;
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

void display_init(){
    pinMode(OLED_RESET, OUTPUT);

    display.begin(0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(32,16);
    display.setTextColor(WHITE);
    display.print(F("Booting..."));
    display.display();

    for(int i=0;i<8;i++){
        display_buffer[i] = (char *)malloc(1);
        display_buffer[i][0] = '\0';
        display_buffer_size[i] = 1;
    }
}

void display_clear_buffer(){
    for(int i=0;i<8;i++){
        memset(display_buffer[i], '\0', display_buffer_size[i]);
        display_line_start[i] = 0;
    }
}

void display_set_buffer_line(int line, const char *str){
    int len = strlen(str);
    display_buffer[line] = (char *)realloc(display_buffer[line], len + 1);
    memcpy(display_buffer[line], str, len);
    display_buffer[line][len] = '\0';
    display_buffer_size[line] = len;
}

void display_set_buffer_line_centered(int line, const char *str){
    int len = strlen(str);
    int spaces = (OLED_MAXLEN - len) / 2;
    display_buffer[line] = (char *)realloc(display_buffer[line], spaces + len + 1);
    if(spaces != 0)
        sprintf(display_buffer[line], "%*c", spaces, ' ');
    memcpy(display_buffer[line]+spaces, str, len);
    display_buffer[line][spaces+len] = '\0';
    display_buffer_size[line] = spaces+len;
}

void display_set_buffer_line(int line, uint8_t *data, uint8_t start, uint16_t len) {
    display_buffer[line] = (char *)realloc(display_buffer[line], len + 1);
    memcpy(display_buffer[line], &data[start], len);
    display_buffer[line][len] = '\0';
    display_buffer_size[line] = len;
}

void display_set_buffer_line(int line, const char *prefix, uint8_t *data, uint8_t start, uint16_t len) {
    int prefix_len = strlen(prefix);
    display_buffer[line] = (char *)realloc(display_buffer[line], prefix_len + len + 1);
    memcpy(display_buffer[line], prefix, prefix_len);
    memcpy(display_buffer[line]+prefix_len, &data[start], len);
    display_buffer[line][prefix_len+len] = '\0';
    display_buffer_size[line] = prefix_len+len;
}

void display_set_header(){
    display.fillRect(0, 0, display.width(), 7, WHITE);
    display.setTextSize(1);
    display.setTextColor(BLACK);
    if(ble_connected){
        // bluetooth logo: super shitty edition
        // s2g this was easier than drawing a BMP
        display.drawTriangle(3, 3, 3, 1, 4, 2, BLACK);
        display.drawTriangle(3, 5, 3, 3, 4, 4, BLACK);
        display.drawLine(2, 2, 3, 3, BLACK);
        display.drawLine(2, 4, 3, 3, BLACK);
    }
    display.println("      LLDPStick");

    // battery sillhouette
    display.fillRect(display.width()-20, 1, 18, 5, BLACK);
    display.fillRect(display.width()-2, 3, 1, 1, BLACK);

    // battery bars
    if(battery_reading >= 585) { // 3.4v
        display.fillRect(display.width()-18, 2, 2, 3, WHITE);
    }

    if(battery_reading >= 636) { // 3.7v
        display.fillRect(display.width()-14, 2, 2, 3, WHITE);
    }

    if(battery_reading >= 687) { // 4.0v
        display.fillRect(display.width()-10, 2, 2, 3, WHITE);
    }

    if(battery_reading >= 705) { // 4.1v
        display.fillRect(display.width()-6, 2, 2, 3, WHITE);
    }
}

void display_set_footer(){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    if(display_buffer_length() > DISPLAY_LINES){
        display.println("< More        Menu >");
    } else {
        display.println("              Menu >");
    }
}

void display_print(){
    char buf[OLED_MAXLEN+1];

    display.clearDisplay();
    display.setCursor(0, 0);

    display_set_header();

    display.setTextSize(1);
    display.setTextColor(WHITE);

    // display lines, potentially scrolling longer ones
    for(int i = display_buffer_top;i<display_buffer_top+DISPLAY_LINES;i++){
        memset(buf, '\0', sizeof(buf));
        int len = strlen(display_buffer[i]);
        if(len <= OLED_MAXLEN){
            // just display the line as-is
            memcpy(&buf, display_buffer[i], len);
            buf[len] = '\0';
        } else {
            // scroll the line slowly to display all text
            memcpy(&buf, &display_buffer[i][display_line_start[i]], OLED_MAXLEN);
            buf[OLED_MAXLEN] = '\0';

            if(millis() - display_last_animation > (display_line_start[i]==0 ? DISPLAY_SCROLL_DELAY*4 : DISPLAY_SCROLL_DELAY) ){
                display_last_animation = millis();
                if(++display_line_start[i] > len){
                    display_line_start[i] = 0;
                }
            }
        }
        display.println(buf);
    }

    display_set_footer();

    display.display();
}

void display_buffer_advance(){
    display_last_animation = millis();
    display_buffer_top++;
    if(display_buffer_top > display_buffer_length()-1)
        display_buffer_top = 0;
}

uint8_t display_buffer_length(){
    uint8_t len = 0;
    for(uint8_t i=0;i<8;i++){
        if(strlen(display_buffer[i]) != 0)
            len++;
    }

    return len;
}
