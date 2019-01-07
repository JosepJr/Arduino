#include <math.h>
#include <dummy.h>
#include <WiFi.h>
#include <esp_system.h>
#include "esp_wifi.h"

#define MAX 556

unsigned long Tempo;
unsigned long Tempo1;
unsigned long Tempo2;
/*uint64_t chipid;

//Funcao que retorna o meu MAC
String getMacAddress() {
	uint8_t baseMac[6];
	// Get MAC address for WiFi station
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	char baseMacChr[18] = {0};
	sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
	return String(baseMacChr);
}
*/

//Acess Point
//const char *ssidAP = "ESP32 Access Point";
//const char *passwordAP = "accesspoint";

//Rede da Minha casa
//const char* ssidClient     = "Republica Virus";
//const char* passwordClient = "Intelcore";

//Rede do Laboratório de Fìsica
const char* ssidClient     = "Fisica";
const char* passwordClient = "Fisic@42";

//Rede do Laboratório da Computacao
//const char* ssidClient     = "AP1REUNI";
//const char* passwordClient = "UeRw76=wR-";

WiFiServer server(80);

void setup(){

    Serial.begin(115200);
    delay(10);

    /*
    Serial.printf("------------------------------------------------------\n");
    Serial.print("Memoria livre no inicio do setup: ");
    Serial.println(ESP.getFreeHeap());
    Serial.println("------------------------------------------------------\n");
    */

    /*  Coisas uteis
      WiFi.mode(m): set mode to WIFI_AP, WIFI_STA, WIFI_AP_STA or WIFI_OFF
      WiFi.localIP() is for STA, WiFi.softAPIP()
    */

    WiFi.mode(WIFI_STA);
    //WiFi.mode(WIFI_AP);
    //WiFi.mode(WIFI_AP_STA);
    //WiFi.mode(WIFI_OFF);

    /*
    //Configurando Como Acess Point
    Serial.println("Configurando Access Point...");
    Serial.println();
    WiFi.softAP(ssidAP, passwordAP);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("Access Point IP Address: ");
    Serial.println(myIP);

    Serial.printf("------------------------------------------------------\n");
    Serial.print("Memoria livre depois de configurar o AP: ");
    Serial.println(ESP.getFreeHeap());
    Serial.printf("------------------------------------------------------\n");
    */

    // Configurando como Cliente
    Serial.println("Configurando como modo Client...");
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssidClient);

    WiFi.begin(ssidClient, passwordClient);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    server.begin();
    Serial.println("Server started");

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    /*Serial.printf("------------------------------------------------------\n");
    Serial.print("Memoria livre depois de configurar o Client: ");
    Serial.println(ESP.getFreeHeap());
    Serial.printf("------------------------------------------------------\n");
    */

    //LED
    pinMode(5, OUTPUT);
    //Relé
    pinMode(18, OUTPUT);

    /*
    Serial.printf("------------------------------------------------------\n");
    Serial.print("Memoria livre no final do Setup: ");
    Serial.println(ESP.getFreeHeap());
    Serial.printf("------------------------------------------------------\n");
    */
}

void loop(){
  /*
   Serial.printf("------------------------------------------------------\n");
   Serial.print("Memoria livre no incio da main(loop): ");
   Serial.println(ESP.getFreeHeap());
   Serial.printf("------------------------------------------------------\n");
  */
   /*
   //Informacoes sobre a ESP32
   Serial.println();
   Serial.printf("MAC ADDRESS: ");
   Serial.println(getMacAddress());
   Serial.println();
   */

   //Ligando os Pinos 5 e 18
   digitalWrite(5, LOW);
   digitalWrite(18, HIGH);
   delay(2000);


   //Vetores de Tensao e Corrente
   float VTensao[MAX];
   float VCorrente[MAX];
   float x;

   //Calculando quanto tempo demora para preencher os valores no vetor de tamanho MAX
   Tempo1 = millis();
   for (int i = 0; i < MAX; i++) {
     VTensao[i] = analogRead(32);
     VCorrente[i] = analogRead(34);
     //Cálculo Aleatorio para consumir tempo. Essa variável faz com que eu possa simular uma leitura de 3 canais ADC,
     //isso serve para que eu possa simular o tempo de 17ms.
     //17 ms é o tempo que a esp32 gasta para preencher 1667 valores (1 ciclo de onda de 60hz). Porém, por questão de hardware,
     //nao é possivel criar 2 vetores (tensao e corrente) de 556 valores + 1 vetor (potencia media).
     //Logo para simular 1667 foi criado 2 vetores de 556 e também a leitra de um canal ADC Aleatorio
     //deste modo eu simulo a leitura de 556x3 e consigo o tempo de 17ms.
     x = analogRead(33);
   }
   Tempo2 = millis();

   //Tempo para preencher os 2 vetores
   Tempo = Tempo2-Tempo1;
   Serial.print("Tempo para leitura dos canais ADC, e preencher os dois vetores: ");
   Serial.print(Tempo);
   Serial.println(" ms");

   //Fazendo a conversão do canal ADC para tensao e corrente. Formula = (3.3)*K/4095
   Tempo1 = millis();
   for(int i = 0; i < MAX; i++){
     VTensao[i] = ((VTensao[i]*(3.3))/4095);//transformacao dos valores
     VCorrente[i] = ((VCorrente[i]*(3.3))/4095);//transformacao dos valores
     VTensao[i] = ((-0.0439)*(VTensao[i]*VTensao[i]*VTensao[i]))+(0.1469*(VTensao[i]*VTensao[i]))+(0.896*(VTensao[i]))+(0.1695); //Correção dos valores usando formula da nao linearidade
     VCorrente[i] = ((-0.0439)*(VCorrente[i]*VCorrente[i]*VCorrente[i]))+(0.1469*(VCorrente[i]*VCorrente[i]))+(0.896*(VCorrente[i]))+(0.1695);//Correção dos valores usando a forma da nao linearidade que esta na tabela
   }
   Tempo2 = millis();
   //Tempo para corrigir os vetores usando a formula da não linearidade
   Tempo = Tempo2-Tempo1;
   //Serial.print(Tempo);
   //Serial.println(" ms");


   //Fazendo a correcao dos valores e a adição dos valores negativos
   Tempo1 = millis();
   for (int i = 0; i < MAX; i++) {
     VTensao[i] = ((VTensao[i]-(1.62))*142.7);
     VCorrente[i] = ((VCorrente[i]-(1.62))*8);
   }
   Tempo2 = millis();

   //Tempo para Converter os valores dos 2 vetores
   Tempo = Tempo2-Tempo1;
   //Serial.print("Tempo conversao usando formula (3.3)*K/4095: ");
   //Serial.print(Tempo);
   //Serial.println(" ms");


   //Calculando a Potencia Instantanea
   float PotenciaInstantanea[MAX];
   for(int i = 0; i < MAX; i++){
     PotenciaInstantanea[i] = VTensao[i] * VCorrente[i];
   }

   //Potencia Instantanea Acomulada
   float PotenciaInstantaneaAcomulada = 0;
   for(int i = 0; i < MAX; i++){
          PotenciaInstantaneaAcomulada = PotenciaInstantaneaAcomulada + PotenciaInstantanea[i];
   }

   //Calculando Potencia Media
   float PotenciaMedia = 0;
   PotenciaMedia = PotenciaInstantaneaAcomulada / MAX;

   //Imprimindo a Potencia Media
   //Serial.println("Potencia Media: ");
   //Serial.println(PotenciaMedia);

   /*
   //Imprimindo vetor Media
   Serial.println("Vetor de Media: \n");
   for(int i = 0; i < MAX; i++){
       Serial.print(PotenciaInstantanea[i]);
       Serial.print(" - ");
   }
   Serial.println();

   //Imprimindo a o vetor de TENSAO na saida Serial
   Serial.println("Vetor de Tensao: \n");
   for(int i = 0; i < MAX; i++){
       Serial.print(VTensao[i]);
       Serial.print(" - ");
   }
   Serial.println();

   //Imprimindo o vetor de CORRENTE na saida Serial
   Serial.println("Vetor de Corrente: \n");
   for(int i = 0; i < MAX; i++){
       Serial.print(VCorrente[i]);
       Serial.print(" - ");
   }
   Serial.println();
   */

   //Calculando Valor eficaz TENSAO
   float ValorEficazTensao = 0;
   for(int i = 0; i < MAX; i++){
        ValorEficazTensao = (ValorEficazTensao + (VTensao[i] * VTensao[i]));
   }
   ValorEficazTensao = ValorEficazTensao/MAX;
   ValorEficazTensao = sqrt(ValorEficazTensao);
   //Serial.print("Valor Eficaz da Tensao: ");
   //Serial.println(ValorEficazTensao);

   //Calculando Valor eficaz CORRENTE
   float ValorEficazCorrente = 0;
   for(int i = 0; i < MAX; i++){
        ValorEficazCorrente = (ValorEficazCorrente + (VCorrente[i] * VCorrente[i]));
   }
   ValorEficazCorrente = ValorEficazCorrente/MAX;
   ValorEficazCorrente = sqrt(ValorEficazCorrente);
   //Serial.print("Valor Eficaz da Corrente: ");
   //Serial.println(ValorEficazCorrente);

   //Calculando Fator de Potencia
   float FatorDePotencia = 0;
   FatorDePotencia = (PotenciaMedia/(ValorEficazTensao*ValorEficazCorrente));
   //Serial.printf("Fator de Potencia: ");
   //Serial.println(FatorDePotencia);


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
    s += "Informacoes Importantes: ";
    s += "<br>";
    s += "Potencia Media: ";
    s += String(PotenciaMedia);
    s += " W<br>";
    s += "Valor Eficaz da Tensao: ";
    s += String(ValorEficazTensao);
    s += " V<br>";
    s += "Valor Eficaz de Corrente: ";
    s += String(ValorEficazCorrente);
    s += " A<br>";
    s += "Fator de Potencia: ";
    s += String(FatorDePotencia);
    s += "<br><br><hr><br>";
    s += "Vetor de Corrente: ";
    s += "<br>";
    for(int i = 0; i < MAX; i++){
      s += " ";
      s += String(VCorrente[i]);
      s += " ";
    }
    s += "<br><br><hr><br>";
    s += "Vetor de tensao: ";
    s += "<br>";
    for(int i = 0; i < MAX; i++){
      s += " ";
      s += String(VTensao[i]);
      s += " ";
    }
    s += "</body></html>";

    // Send the response to the client
    client.print(s);
    Serial.println("Client disconnected");

    //Desligando os Pinos 5 e 18
    digitalWrite(5, HIGH);
    digitalWrite(18, LOW);
    delay(10000);

    /*
    //Informacoes sobre a ESP32 depois de rodar todo o programa
    Serial.printf("------------------------------------------------------\n");
    Serial.print("Memoria livre no final da main(loop): ");
    Serial.println(ESP.getFreeHeap());
    Serial.printf("------------------------------------------------------\n");
    */
}
