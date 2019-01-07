#include <dummy.h>
#define MAX 850
#include <WiFi.h>
#include <esp_system.h>


//Rede da Minha casa
//const char* ssid     = "Republica Virus";
//const char* password = "Intelcore";

//Rede do Laboratório de Fìsica 
//const char* ssid     = "Fisica";
//const char* password = "Fisic@42";

//Rede do Laboratório da Computacao
const char* ssid     = "AP1REUNI";
const char* password = "UeRw76=wR-";

//WiFiServer server(80);

void setup(){
    Serial.begin(115200);
    delay(10);

    Serial.printf("------------------------------------------------------\n");
    Serial.print("Memoria livre no setup: ");
    Serial.println(ESP.getFreeHeap());

    /*// We start by connecting to a WiFi network
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
    */
    //LED
    pinMode(5, OUTPUT);
    //Relé
    pinMode(18, OUTPUT);
}

void loop(){

   //Informacoes sobre a ESP32
   Serial.printf("------------------------------------------------------\n");
   Serial.print("Memoria livre na main antes do vetor ser preenchido: ");
   Serial.println(ESP.getFreeHeap());
 
   //Ligando os Pinos 5 e 18
   digitalWrite(5, LOW);
   digitalWrite(18, HIGH);
   delay(2000);
   
   /*    
   //Vetores de Tensao e Corrente 
   int VTensao[MAX];
   int VCorrente[MAX];
   
   //Calculando quanto tempo demora para preencher os valores no vetor de tamanho MAX
   int Tempo1 = millis();  
   for (int i = 0; i < MAX; i++) {
     VTensao[i] = analogRead(32);
     VCorrente[i] = analogRead(34);
   }
   int Tempo2 = millis();
    
   //Tempo para preencher os 2 vetores
   int Tempo = Tempo2-Tempo1;
   Serial.print("Tempo para preencher os dois vetores: ");
   Serial.print(Tempo);
   Serial.println("ms");
     
   //Imprimindo a o vetor de TENSAO na saida Serial
   Serial.println("Vetor de Tensao: \n");
   for(int i = 0; i < MAX; i++){
       Serial.print((VTensao[i]*(3.3))/4095);
       Serial.print(" - ");
   }
   Serial.println();
  
   //Imprimindo o vetor de CORRENTE na saida Serial
   Serial.println("Vetor de Corrente: \n");
   for(int i = 0; i < MAX; i++){
       Serial.print((VCorrente[i]*(3.3))/4095);
       Serial.print(" - ");      
   }
   Serial.println();
   */
   //Desligando os Pinos 5 e 18
   digitalWrite(5, HIGH);
   digitalWrite(18, LOW);

   delay(2000);

   //Informacoes sobre a ESP32 depois de rodar todo o programa
   Serial.printf("------------------------------------------------------\n");
   Serial.print("Memoria livre no final do codigo: ");
   Serial.println(ESP.getFreeHeap());
   
   //10 Segundos até a proxima contagem
   //Serial.println("10 Segundos até a Próxima amostragem de dados... ");
   //delay(10000);  
}
