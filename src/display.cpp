#include "config.h"
#include "display.h"
#include "battery.h"

char *display_buffer[OLED_LINES];
int display_buffer_length[OLED_LINES] = {0};
int display_line_start[OLED_LINES] = {0};
uint32_t last_animation, last_header_animation = 0;
Adafruit_SSD1306 display(128, SSD1306_LCDHEIGHT, &Wire, OLED_RESET);

void display_init(){
    pinMode(OLED_RESET, OUTPUT);

    display.begin(0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(32,16);
    display.setTextColor(WHITE);
    display.print(F("Booting..."));
    display.display();

    for(int i=0;i<OLED_LINES;i++){
        display_buffer[i] = (char *)malloc(1);
        display_buffer[i][0] = '\0';
        display_buffer_length[i] = 1;
    }
}

void clearBuffer(){
    for(int i=0;i<OLED_LINES;i++){
        memset(display_buffer[i], '\0', display_buffer_length[i]);
        display_line_start[i] = 0;
    }
}

void setBufferLine(int line, const char *str){
    int len = strlen(str);
    display_buffer[line] = (char *)realloc(display_buffer[line], len + 1);
    memcpy(display_buffer[line], str, len);
    display_buffer[line][len] = '\0';
    display_buffer_length[line] = len;
}

void setBufferLine(int line, uint8_t *data, uint8_t start, uint16_t len) {
    display_buffer[line] = (char *)realloc(display_buffer[line], len + 1);
    memcpy(display_buffer[line], &data[start], len);
    display_buffer[line][len] = '\0';
    display_buffer_length[line] = len;
}

void setBufferLine(int line, const char *prefix, uint8_t *data, uint8_t start, uint16_t len) {
    int prefix_len = strlen(prefix);
    display_buffer[line] = (char *)realloc(display_buffer[line], prefix_len + len + 1);
    memcpy(display_buffer[line], prefix, prefix_len);
    memcpy(display_buffer[line]+prefix_len, &data[start], len);
    display_buffer[line][prefix_len+len] = '\0';
    display_buffer_length[line] = prefix_len+len;
}

void setHeader(){
    display.fillRect(0, 0, display.width(), 7, WHITE);
    display.setTextSize(1);
    display.setTextColor(BLACK);
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

void setFooter(){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("              Menu >");
}

void printDisplay(uint8_t text_size){
    char buf[OLED_MAXLEN+1];

    display.clearDisplay();
    display.setCursor(OLED_HORIZONTAL_OFFSET, OLED_VERTICAL_OFFSET);

    setHeader();

    display.setTextSize(text_size);
    display.setTextColor(WHITE);

    // display lines, potentially scrolling longer ones
    for(int i = 0;i<OLED_LINES;i++){
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

            if(millis() - last_animation > (display_line_start[i]==0 ? DISPLAY_SCROLL_DELAY*4 : DISPLAY_SCROLL_DELAY) ){
                last_animation = millis();
                if(++display_line_start[i] > len){
                    display_line_start[i] = 0;
                }
            }
        }
        display.println(buf);
    }

    setFooter();

    display.display();
}

void printDisplay(){
    printDisplay(1);
}

