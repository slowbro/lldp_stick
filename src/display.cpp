#include "config.h"
#include "display.h"

char *display_buffer[OLED_LINES];
int display_buffer_length[OLED_LINES] = {1};
int display_line_start[OLED_LINES] = {0};
unsigned long last_animation = 0;
int bat = 680;

void clearBuffer(){
    for(int i=0;i<OLED_LINES;i++){
        memset(&display_buffer[i], '\0', sizeof(*display_buffer[i])*display_buffer_length[i]);
        display_line_start[i] = 0;
    }
}

void setBufferLine(int line, char *str){
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

void setBufferLine(int line, char *prefix, uint8_t *data, uint8_t start, uint16_t len) {
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
    //display.println("      LLDPStick");
    if(millis() - last_animation > 250){
        last_animation = millis();
        bat = analogRead(VDIV);
        display.println(bat*0.0066, 2);
    } else {
        display.println(bat*0.0066, 2);
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

