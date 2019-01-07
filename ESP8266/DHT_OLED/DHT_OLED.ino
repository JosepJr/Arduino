#include "DHT.h"
#define DHTPIN 2     // Porta
#define DHTTYPE DHT22
#include <SPI.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

DHT dht(DHTPIN, DHTTYPE);
MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C declaration

void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(100);

  dht.begin();
  oled.begin();

  double h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  double t = dht.readTemperature();


  oled.clear(PAGE); //clear will clear the Arduino's display buffer.
  oled.clear(ALL); // Clear the display's memory (gets rid of artifacts)
  oled.setFontType(0);  // Set the text to small (10 columns, 6 rows worth of characters).

  oled.print("Humidity: ");
  oled.print(h, 2);
  oled.print(" %\n\n");
  oled.print("Temp:\n");
  oled.print(t, 2);
  oled.print(" *C");

  Serial.println();
  Serial.print("Umidade: ");
  Serial.print(h, 2);
  Serial.print(" %\n\n");
  Serial.print("Temperatura: ");
  Serial.print(t, 2);
  Serial.println(" *C");

  oled.display();

  delay(1900);
}
