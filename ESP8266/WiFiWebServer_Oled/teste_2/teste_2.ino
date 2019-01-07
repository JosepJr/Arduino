// Test for minimum program size.

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#define SCLPIN  5
#define SDAPIN  4

SSD1306AsciiWire oled;

void setup() {
  Wire.begin(SDAPIN, SCLPIN);          //sda, scl 

  delay(50);
               
  oled.begin(&MicroOLED64x48, I2C_ADDRESS);
  oled.setFont(lcd5x7);
  oled.clear();
  oled.print("0123456789");
}
void loop() {}
