#ifndef CONFIG_H
#define CONFIG_H

#define PERIPH 1 // rev 2
#define WIZ_RESET 1 // 8 for rev 2
#define OLED_RESET 7
#define WIZ_CS 5
#define LBTN 22
#define RBTN 6
#define VDIV 31

#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_MAX_LINES 4
#define OLED_LINES 2
#define OLED_MAXLEN 21
#define OLED_VERTICAL_OFFSET 0
#define OLED_HORIZONTAL_OFFSET 0

#define BUTTON_DEBOUNCE_DELAY 50
#define DISPLAY_SCROLL_DELAY 250

#define BATTERY_READ_DELAY 1000
#define BATTERY_COEFFICIENT 0.00582

#endif
