#include "DHT.h"
#define DHTPIN 2     // Porta
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>

const char* ssid = "ALA_A";
const char* password = "U=?wRv-!Tx";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(10);

  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1998);
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {  
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("New client");
  while(!client.available()){
    delay(1);
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
    
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
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

