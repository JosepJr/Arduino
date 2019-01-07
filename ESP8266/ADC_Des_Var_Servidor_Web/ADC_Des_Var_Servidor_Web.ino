#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <math.h>
#define MAX 5


const char *ssid = "DESVARSERVER";
const char *password = "1";

WiFiServer server(80);
/* Just a little test message.  Go to http://192.168.4.1 in a web browser */


void setup() {
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");
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

   int val = 0;
   int i, j;
   double VetMedia[MAX] = {0, 0, 0, 0, 0};
   int VetAmostras[MAX] = {5, 10, 2, 3, 7};
  
   //Preenchendo o Vetor Media com a media das amostras analizadas por dia
   double Aux = 0;
   for(j = 0; j < MAX; j++){
    for(i = 0; i < VetAmostras[j]; i++){
      val = analogRead(0);
      Aux = Aux + val;
      delay(50);
    }
    VetMedia[j] = (Aux/VetAmostras[j]);
    Aux = 0;
   }

   /* Teste -------------------
   for(i = 0; i < MAX; i++){
      Serial.print(VetMedia[i], 2);
      Serial.printf(" ");    
   }
   Serial.println();
   //--------------------------*/
   
   //MediaAritmetica
   int MediaAritmetica =  0;
   for(i = 0; i < MAX; i++){
      MediaAritmetica = MediaAritmetica+VetMedia[i];    
   }
   MediaAritmetica = (MediaAritmetica/MAX);

   //Calculando a Variancia
   
   double Variancia = 0;
   for(i = 0; i < MAX; i++){
     Variancia = (pow((VetMedia[i] - MediaAritmetica), 2) + Variancia);
   }
   Variancia = (Variancia/MAX);
      
   //Calculando Desvio Padrao
   double DesvioPadrao = sqrt(Variancia);
     
  // Prepare the response
   String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta http-equiv='refresh' content='2'></head>\r\n";
  s += "Variancia: ";                                                                          
  s += String(Variancia);
  s += "<br>Desvio Padrao: ";
  s += String(DesvioPadrao);
  s += "<br>";
  s += "</html>\n";    

  // Send the response to the client
  client.print(s);
  client.flush();
 
}

