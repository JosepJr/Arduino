/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#define MAX 250
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
}

//int value = 0;

void loop()
{
     delay(2000);
    
     //vetores de Tensao e Corrente 
     int VTensao[MAX];
     int VCorrente[MAX];
     //Calculando quanto tempo demora para preencher os valores no vetor de tamanho MAX
    
    int Tempo1 = millis();  
    for (int i = 0; i < MAX; i++) {
      VTensao[i] = analogRead(32);
    }
    int Tempo2 = millis();
    int TempoTensao = Tempo2-Tempo1;  
    Serial.println();
    Serial.println("-------Tempo para preencher o vetor com 125 valores de Tesao--------");    
    Serial.println(TempoTensao);
    Serial.println("--------------------------------------------------------------------");
    
    int Tempo3 = millis();  
    for (int i = 0; i < MAX; i++) {
      VCorrente[i] = analogRead(34);
    }
    int Tempo4 = millis();
    int TempoCorrente = Tempo4-Tempo3;  
    Serial.println();
    Serial.println("-------Tempo para preencher o vetor com 125 valores de Corrente--------");    
    Serial.println(TempoCorrente);
    Serial.println("-----------------------------------------------------------------------");
    

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
    s += "Tempo para preencher vetor de tensao: ";
    s += String(TempoTensao);
    s += "<br>";
    s += "Vetor de Tesao: ";
    for(int i = 0; i < MAX; i++){
       s += String(((VTensao[i]*(3.3))/4095));
       s += " - ";
    }
    s += "<br><br><hr><br>";
    s += "Tempo para preencher vetor de corrente: ";
    s += String(TempoCorrente);
    s += "<br>";
    s += "Vetor de Corrente: ";
    for(int i = 0; i < MAX; i++){
       s += String(((VCorrente[i]*(3.3))/4095));
       s += " - ";
    } 
    s += "</body></html>"; 
    // Send the response to the client
    client.print(s);
    delay(2000); 
    Serial.println("Client disconnected"); 
    
          
    /*delay(5000);
    ++value;

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/input/";
    url += streamId;
    url += "?private_key=";
    url += privateKey;
    url += "&value=";
    url += value; 

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection"); */
}

