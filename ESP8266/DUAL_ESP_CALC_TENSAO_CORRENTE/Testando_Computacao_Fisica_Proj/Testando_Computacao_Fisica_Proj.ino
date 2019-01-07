#define MAX 250

#include <ESP8266WiFi.h>

const char* ssid = "Fisica";
const char* password = "Fisic@42";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

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

  int vector[MAX];
  //Calculando quanto tempo demora para preencher os valores no vetor de tamanho MAX
  
  int t1 = millis();  
  for (int i = 0; i < MAX; i++) {
    vector[i] = analogRead(0);
  }
  int t2 = millis();
  int tempo = t2-t1;  
  Serial.println();
  Serial.println("-------Tempo para preencher o vetor--------");
  Serial.println(t1);
  Serial.println(t2);
  
  Serial.println(t2 - t1);
  Serial.println("-------------------------------------------");

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("New client");
  while (!client.available()) {
    delay(1);
  }

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta http-equiv='refresh' content='2'></head>\r\n";
  s += "Tempo: ";
  s += String(tempo);
  s += "<br>";
  s += "Vetor: ";
  for(int i = 0; i < MAX; i++){
     s += String(((vector[i]*(3.3))/1024));
     s += " - ";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

}
