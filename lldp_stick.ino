#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "w5100.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define LEFT_BTN 0
#define RIGHT_BTN 2
#define WIZNET_RESET 5
#define OLED_RESET 6
#define STAT1 14
#define STAT2 15
#define PG 16

#define SS 9
#define MOSI 10
#define MISO 11
#define SCK 12

#define GRN_LED 20
#define RED_LED 21

#define SSD1306_LCDHEIGHT 32
Adafruit_SSD1306 display(OLED_RESET);
#define OLED_LINES 2
#define OLED_MAXLEN 21
// offsets for odd screens
#define OLED_VERTICAL_OFFSET 16
#define OLED_HORIZONTAL_OFFSET 0

char *display_buffer[OLED_LINES];
int display_line_start[OLED_LINES];

bool left_btn = false;
bool right_btn = false;

const byte mac[] = { 0xae, 0x03, 0xf3, 0xc7, 0x08, 0x78 };
uint8_t network_buffer[512];
Wiznet5100 w5100;

struct PDUInfo {

  uint8_t ChassisIdStart;
  // TLV 1
  uint8_t ChassisIdSubtype;
  uint16_t ChassisIdLength;
  // TLV 2
  uint8_t PortIdStart;
  uint8_t PortIdSubtype;
  uint16_t PortIdLength;
  // TLV 4
  uint8_t PortDescriptionStart;
  uint16_t PortDescriptionLength;
  // TLV 5
  uint8_t SystemNameStart;
  uint16_t SystemNameLength;
  // TLV 6
  uint8_t SystemDescriptionStart;
  uint16_t SystemDescriptionLength;

};

struct TLV_Org {

  // TLV 127
  uint8_t derp;

};

void setup() {
  Serial.begin(115200);
  Serial.println(F("initializing"));

  // set up inputs
  pinMode(LEFT_BTN, INPUT);
  pinMode(RIGHT_BTN, INPUT);
  pinMode(STAT1, INPUT);
  pinMode(STAT2, INPUT);
  pinMode(PG, INPUT);

  // set up outputs
  pinMode(WIZNET_RESET, OUTPUT);
  pinMode(OLED_RESET, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GRN_LED, OUTPUT);

  // set up the display and clear it
  display.begin(SSD1306_CHARGEPUMP, 0x3C);
  display.clearDisplay();

  // enable PCINTs
  cli();
  // TBD
  sei();

  // run the boot routine
  boot();
}


void loop() {
  // power management
  checkPower();

  // buttons
  if(digitalRead(LEFT_BTN) == HIGH && !left_btn){
    left_btn = true;
    Serial.println(F("left!"));
    btnLeft();
  }
  if(digitalRead(LEFT_BTN) == LOW && left_btn){
    left_btn = false;
  }
  if(digitalRead(RIGHT_BTN) == HIGH && !right_btn){
    right_btn = true;
    Serial.println(F("right!"));
    btnRight();
  }
  if(digitalRead(RIGHT_BTN) == LOW && right_btn){
    right_btn = false;
  }
  
  // check link
  if(!w5100.hasLink()){
    setBufferLine(1, F("* No Link *"));
    printDisplay();
    return;
  }
  // receive and ethernet frame and process it
  uint16_t len = w5100.readFrame(network_buffer, sizeof(network_buffer));
  if (len > 0 ) {
    Serial.print(F("Ethertype"));
    Serial.print(network_buffer[12]);
    Serial.print(F(","));
    Serial.print(network_buffer[13]);
    Serial.print(F(": len "));
    Serial.println(len);
  }
  // we only care about ethertype 0x88CC (LLDP)
  if ( len > 12 && network_buffer[12] == 136 && network_buffer[13] == 204 ) {
    PDUInfo pinfo;
    processLLDP(network_buffer, len, &pinfo);
    setBufferLine(0, network_buffer, pinfo.SystemNameStart, pinfo.SystemNameLength);
    setBufferLine(1, network_buffer, pinfo.PortIdStart, pinfo.PortIdLength);
    printDisplay();
  }
}

bool processLLDP(uint8_t *data, uint16_t len, PDUInfo *pinfo) {
  uint8_t tlv_type;
  uint16_t tlv_len = 0;

  // starting at byte 14, start discovering TLVs and their length
  for (uint16_t pos = 14; pos < len; pos++) {
    // extract the headers for a new TLV
    uint16_t tlv = ((uint16_t)data[pos] << 8) + data[++pos];
    // first 7 bits are the TLV ID
    tlv_type = tlv >> 9;
    // last 9 bits are the TLV length
    tlv_len =  tlv & 0b0000000111111111;
    
    /*Serial.print(F("\nFound tlv "));
    Serial.print(tlv_type, DEC);
    Serial.print(F(" ("));
    Serial.print(tlv);
    Serial.print(F(") len "));
    Serial.println(tlv_len);*/

    // store the TLV info in pinfo (PDUInfo struct)
    switch (tlv_type) {
      case 0:
        // END OF PDU
        // skip to the end to terminate the loop
        pos = len;
        break;
      case 1:
        pinfo->ChassisIdSubtype = data[pos + 1];
        pinfo->ChassisIdStart = pos + 2;
        pinfo->ChassisIdLength = tlv_len - 1;
        break;
      case 2:
        pinfo->PortIdSubtype = data[pos + 1];
        pinfo->PortIdStart = pos + 2;
        pinfo->PortIdLength = tlv_len - 1;
        break;
      case 5:
        pinfo->SystemNameStart = pos + 1;
        pinfo->SystemNameLength = tlv_len;
      default:
        Serial.print(F("Ignoring TLV ")); Serial.println(tlv_type);
    }
    pos += tlv_len;
  }
  return true;
}

void setBufferLine(int line, __FlashStringHelper *str){
  setBufferLine(line, (char *)str);
}

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

void printDisplay(){
  printDisplay(1);
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

void btnLeft(){
    digitalWrite(GRN_LED, HIGH);
    delay(500);
    digitalWrite(GRN_LED, LOW);
}

void btnRight(){
    digitalWrite(RED_LED, HIGH);
    delay(500);
    digitalWrite(RED_LED, LOW);
}

// 'boot' the system (from off/powersave)
void boot(){
  // initialize the wiznet chip
  digitalWrite(WIZNET_RESET, HIGH);

  // turn on the green led
  digitalWrite(GRN_LED, HIGH);
  
  // display a welcome message
  display.setTextSize(2);
  display.setCursor(32,OLED_VERTICAL_OFFSET);
  display.setTextColor(WHITE);
  display.println(F("Hello!"));
  display.display();
  delay(1000);

  // initialize the ethernet controller
  if(!w5100.begin(mac)){
    digitalWrite(GRN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    Serial.println("phy begin err");
    // display a message and wait forever
    display.clearDisplay();
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.println(F("E01 PHY INIT ERR"));
    display.display();
    while(true) delay(1000);
  }

  // fill the first line of the display with a message
  setBufferLine(0, F("Waiting for LLDP.."));
  printDisplay();
  digitalWrite(GRN_LED, LOW);
}

// manage LEDs for the power management chip
void checkPower(){
  
}

// enter low power mode ('power off')
void powerdown(){
  // hold the wiznet in reset
  digitalWrite(WIZNET_RESET, LOW);

  // clear the screen and hold the controller in reset
  display.clearDisplay();
  display.display();
  digitalWrite(OLED_RESET, LOW);

  // enable the button inturrupts

  // sleep the cpu
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
  sleep_disable();
  
}

