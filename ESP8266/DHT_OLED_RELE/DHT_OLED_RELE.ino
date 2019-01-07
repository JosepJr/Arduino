#include "DHT.h"
#define DHTPIN 2     // Porta
#define DHTTYPE DHT22
#include <SPI.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "Eletrotecnica";
const char *password = "12345";

WiFiServer server(80);

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

DHT dht(DHTPIN, DHTTYPE);
MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C declaration

void setup() {
  Serial.begin(115200);
    
  Serial.println();
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");
  
  //prepare GPIO2
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);    
}

void loop() {

   // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }    
  
  delay(1);
  
  dht.begin();
  oled.begin();
    
  double h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  double t = dht.readTemperature();


  oled.clear(PAGE); //clear will clear the Arduino's display buffer.
  oled.clear(ALL); // Clear the display's memory (gets rid of artifacts)
  oled.setFontType(0);  // Set the text to small (10 columns, 6 rows worth of characters).

  oled.print("Humidity: ");
  oled.print(h,2);
  oled.print(" %\n\n");
  oled.print("Temp:\n");
  oled.print(t,2);
  oled.print(" *C");
  oled.display();

  // Prepare the response
   String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta http-equiv='refresh' content='2'></head>\r\n";
  s += "Humidity: ";                                                                          
  s += String(h);
  s += "%<br>Temperature: ";
  s += String(t);
  s += "*C<br>";
  s += "</html>\n";
  
  // Send the response to the client
  client.print(s);


  if(h < 60){
    digitalWrite(5, LOW);
  }
  
  if(h >= 60){
    digitalWrite(5, HIGH);    
  }
  
  
  delay(1998);
}
