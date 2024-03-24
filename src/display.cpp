#include "config.h"
#include "display.h"

char *display_buffer[OLED_LINES];
int display_line_start[OLED_LINES];

void setBufferLine(int line, char *str){
  int len = strlen(str);
  display_buffer[line] = (char *)realloc(display_buffer[line], (len + 1) * sizeof(char));
  memset(display_buffer[line], 0, len + 1);
  display_buffer[line] = str;
  display_buffer[line][len+1] = '\0';
}

void setBufferLine(int line, uint8_t *data, uint8_t start, uint16_t len) {
  display_buffer[line] = (char *)realloc(display_buffer[line], (len + 1) * sizeof(char));
  memset(display_buffer[line], 0, len + 1);
  for (uint16_t pos=start,i=0;pos<start+len;pos++,i++){
    display_buffer[line][i] = (char)data[pos];
  }
  display_buffer[line][len+1] = '\0';
}

void printDisplay(uint8_t size){
  char buf[OLED_LINES][OLED_MAXLEN+1];

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(OLED_HORIZONTAL_OFFSET, OLED_VERTICAL_OFFSET);
  display.setTextSize(size);

  // chop up the buffer
  for(int i = 0;i<OLED_LINES;i++){
    memset(&buf[i], 0, OLED_MAXLEN+1);
    int len = strlen(display_buffer[i]);
    if(len < OLED_MAXLEN){
      memcpy(buf[i], &display_buffer[i][display_line_start[i]], len);
      buf[i][len+1] = '\0';
    } else {
      memcpy(buf[i], &display_buffer[i][display_line_start[i]], OLED_MAXLEN);
      buf[i][OLED_MAXLEN+1] = '\0';
    }
    display.println(buf[i]);
    free(buf[i]);
  }

  display.display();
}

void printDisplay(){
  printDisplay(1);
}

