
#include <ESP8266WiFi.h>

const char* ssid = "Republica Virus.exe";
const char* password = "Intelcore";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
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
  
  uint32_t realSize = ESP.getFlashChipRealSize();
  
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
  
    
  // Prepare the response
   String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "Flash chip ID: ";
  s += String(ESP.getFlashChipId());
  s += "<br>Flash chip size (in bytes): ";
  s += String(ESP.getFlashChipSize());
  s += "<br>Flash chip speed (in Hz): ";
  s += String(ESP.getFlashChipSpeed());
  s += "<br>Flash real size: ";
  s += String(realSize);
  s += "</html>\n";
  
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

