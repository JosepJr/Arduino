/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#define MAX 850
#include <WiFi.h>

const char* ssid     = "Fisica";
const char* password = "Fisic@42";


WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    server.begin();
    Serial.println("Server started");

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    //Relé e LED
    pinMode(5, OUTPUT);
    digitalWrite(5, LOW); 

}

//int value = 0;

void loop()
{
     delay(2000);
     digitalWrite(5, HIGH);
         
     //vetores de Tensao e Corrente 
     int VTensao[MAX];
     int VCorrente[MAX];
     //Calculando quanto tempo demora para preencher os valores no vetor de tamanho MAX
    
    int Tempo1 = millis();  
    for (int i = 0; i < MAX; i++) {
      VTensao[i] = analogRead(32);
      VCorrente[i] = analogRead(34);
    }
    int Tempo2 = millis();
    int Tempo = Tempo2-Tempo1;  

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
    
    //<meta http-equiv='refresh' content='2'>      Conection: close
    // Prepare the response
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta charset='UTF-8'></head><body>\r\n";
    s += "Tempo para preencher os 2 vetores: ";
    s += String(Tempo);
    s += "<br>";
    s += "Vetor de Corrente: ";
    for(int i = 0; i < MAX; i++){
       s += String(((VTensao[i]*(3.3))/4095));
       s += " - ";
    }
    s += "<br><br><hr><br>";
    s += "Vetor de Tensao: ";
    for(int i = 0; i < MAX; i++){
       s += String(((VCorrente[i]*(3.3))/4095));
       s += " - ";
    } 
    s += "</body></html>"; 
    // Send the response to the client
    client.print(s);
    delay(2000);
    digitalWrite(5, LOW);  
    Serial.println("Client disconnected"); 

}

