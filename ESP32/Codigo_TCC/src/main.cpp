//------------------------------------------------------------------------------
/*

FALAR SOBRE PROS E CONTRAS DA DETECÇÃO DE ZEROS


//------------------------------------------------------------------------------
Comentários Relevantes:

Comandos via terminal Raspberry pi para MQTT
mosquitto_sub -h [ip] -p [porta] -u[usuario] -P [senha] -t [local do post /algumacoisa/etc]
mosquitto_pub -h [ip] -p [porta] -u[usuario] -P [senha] -t [local do post /algumacoisa/etc] -m [mensagem]

Configurações de funcionamento do módulo WiFi
‎server.begin(): Diz ao servidor para começar a escutar para conexões de entrada. ‎(SETUP)

WiFi.mode(m): set mode to WIFI_AP, WIFI_STA, WIFI_AP_STA or WIFI_OFF
WiFi.macAddress(mac)‎ é para STA, para AP.‎WiFi.softAPmacAddress(mac).
WiFi.localIP()‎, WiFi.softAPIP();: Pega o ‎‎WiFi‎‎ endereço IP.‎
AP.‎WiFi.softAPIP():   AP.‎WiFi.softAPIP():1 Parametro configura uma rede aberta,
AP.‎WiFi.softAPIP(ssid, password, channel, hidden, max_connection) com várias configurações. Ex: WiFi.softAP(ssid, password)
WiFi.printDiag(Serial)‎ irá imprimir algumas informações de diagnóstico da rede.(a senha deve ser pelo menos 8 caracteres)

Funcao para pegar a memória livre
ESP.getFreeHeap()



*/
//Bibliotecas que serão necessárias para o total funcionamento do código
#include <Arduino.h>
#include <math.h>
#include <dummy.h>
#include <WiFi.h>
#include <esp_system.h>
#include "esp_wifi.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiServer.h>

//------------------------------------------------------------------------------
//Constantes Necessárias

#define MAX 556   //Tamanho do vetor que ira coletar os dados para calcular um ciclo de onda
#define AUX 5
//Constantes usadas pra medir o tempo e o custo do processamento dos dados.
//unsigned long Tempo;
//unsigned long Tempo1;
//unsigned long Tempo2;

//Vetores de tesao, corrente para uso global do sistema
float vetorTensao[MAX];
float vetorCorrente[MAX];
float potenciaMedia;
float mediaPotenciaMedia;
float valorEficazTensao = 0;
float mediaValorEficazTensao;
float valorEficazCorrente = 0;
float mediaValorEficazCorrente;
float fatorDePotencia;
float mediaFatorDePotencia;
String receiver; //texto recebido do MQTT
float lixo;
int contadorMediaMovel = 0;
float vetorMediaPotenciaMedia[AUX];
float vetorMediaValorEficazTensao[AUX];
float vetorMediaValorEficazCorrente[AUX];
float vetorMediaFatorDePotencia[AUX];

//Configuração da rede WiFi
//const char* ssid = "Fisica";
//const char* password = "Fisic@42";
//const char* mqttServer = "169.254.167.230";

//Configuração da rede WiFi 2
const char* ssid = "UnderGrounD2";
const char* password = "999579007";
const char* mqttServer = "192.168.43.202";

//Configuração da rede WiFi 3
//Rede do Laboratório da Computacao
//const char* ssid     = "AP1REUNI";
//const char* password = "UeRw76=wR-";
//const char* mqttServer = "192.168.0.6";

//Configuração do MQTT
const int mqttPort = 1883;   //Porta
const char* mqttUser = "tccjosep";  //Usuário
const char* mqttPassword = "tcc"; //Senha
const char* publicar = "/publisher";  //Local de publicação
const char* inscrever = "/subscriber"; //Local de Leitura dos dados

WiFiServer server(80); //‎Cria um servidor que escuta para conexões de entrada na porta especificada. ‎
WiFiClient cliente; //Cliente que ira acessar o servidor local


//Cliente MQTT
WiFiClient mqttClient; //‎Cria um cliente que pode conectar a um especificado IP
// porta conforme definido em ‎‎client.connect()‎‎.
PubSubClient client(mqttClient); //Esta biblioteca fornece um cliente para fazer
//simples publicação/assinatura de mensagens com um servidor que suporta MQTT



//Local onde ficara o cabeçalho de todas as funções do programa.
//As funções completas estarão implemenadas no final do código

void publicandoJsonMQTT();
void conectarRedeWiFi();
void conectarServidorMQTT();
void realizarAcao();
void callback(char* topic, byte* payload, unsigned int length);
String getMacAddress();
void lerCanaisADC();
void converterLeituraADC();
void correcaoValores();
void calculoPotencias();
void calculoValorEficazTensao();
void calculoValorEficazCorrente();
void calculoFatorDePotencia();
void calculoDeMediaCircular();
void escreverServidorWeb();

//Setando todas as configurações necessárias
void setup() {
     Serial.begin(115200);

     WiFi.mode(WIFI_STA); //Setando o modo como o WiFi vai funcionar
     //WiFi.mode(WIFI_AP);
     //WiFi.mode(WIFI_AP_STA);
     //WiFi.mode(WIFI_OFF);


     WiFi.begin(ssid, password); //Iniciando o hardware Wifi e conectando a rede.
     server.begin(); //Iniciando Servidor WEB na ESP
     conectarRedeWiFi();  //Conectando na rede WiFi 192.168.0.12



     //SETANDO OS PINOS A SEREM UTILIZADOS
     pinMode(5, OUTPUT); //LED
     pinMode(18, OUTPUT); //RELE


     //Setando as configurações MQTT usando a biblioteca PubSub
     client.setServer(mqttServer, mqttPort);
     client.setCallback(callback); //Funcao que vai ler os dados do canal subscribe definido.

}

void loop() {

     if(contadorMediaMovel == 5){
          contadorMediaMovel = 0;
     }
     client.loop(); //Função que faz um "reboot" no cliente (PubSub) e recarrega
     //todas as informações. É muito util no uso do MQTT ficar lendo dados de algum lugar.

     //Chamada das funções que realizaram todos os processos e cálculos.
     conectarRedeWiFi();
     conectarServidorMQTT();
     calculoDeMediaCircular();
     publicandoJsonMQTT();
     delay(3000);

     escreverServidorWeb();


     client.loop();
     contadorMediaMovel++;
}

//Funções usadas no programa e algumas funções úteis

//Criar Arquivo json para enviar via mqtt
void publicandoJsonMQTT(){
     //300
     StaticJsonBuffer<300> JSONbuffer;
     JsonObject& JSONencoder = JSONbuffer.createObject();

     JSONencoder["device"] = "LOLIN32 PRO";
     JSONencoder["sensorType"] = "Energy";
     JsonArray& values = JSONencoder.createNestedArray("values");
     values.add(mediaPotenciaMedia/5);
     values.add(mediaValorEficazTensao/5);
     values.add(mediaValorEficazCorrente/5);
     values.add(mediaFatorDePotencia/5);
     values.add(contadorMediaMovel);

     //100
     char JSONmessageBuffer[100];
     JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

     //impressao de teste
     Serial.println("Imprimindo JSON: ");
     Serial.println(JSONmessageBuffer);

     //imprimindo no mqtt
     if (client.publish(publicar, JSONmessageBuffer) == true) {
          Serial.println("Success sending message");
     } else {
          Serial.println("Error sending message");
     }
}

//Conectar a rede Wifi
void conectarRedeWiFi(){
     if(WiFi.status() != WL_CONNECTED){
          Serial.printf("Conectando na rede WiFi %s \n", ssid);
          while (WiFi.status() != WL_CONNECTED) {
               delay(500);
               Serial.print(".");
          }
          Serial.printf("\nConexão com a rede estabelecida...\n");
          Serial.print("IP ESP32: ");
          Serial.println(WiFi.localIP());
          Serial.println("\n\n");
     }
}

//Conectar no servidor MQTT da rede Wifi
void conectarServidorMQTT(){
     while (!client.connected()) {
          Serial.println("Conectando ao servidor MQTT...");
          if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
               Serial.println("Conectado");
               client.subscribe(inscrever);
          }
          else{
               Serial.print("Falhou com o estado ");
               Serial.println(client.state());
               delay(2000);
          }
     }
     client.subscribe(inscrever);
}

//Realizar acção de acordo com a string recebida do canal de inscricao do MQTT
void realizarAcao(){
     if(receiver.equalsIgnoreCase("PIN05ON")){
          digitalWrite(5, LOW);
          digitalWrite(18, HIGH);
          client.publish(publicar, String("Led Ligado").c_str(), true);
     }
     if(receiver.equalsIgnoreCase("PIN05OFF")){
          digitalWrite(5, HIGH);
          digitalWrite(18, LOW);
          client.publish(publicar, String("Led Desligado").c_str(), true);
     }
     receiver = "";
}

//Função para pegar no topico definido a string e Armazenar em uma variável
void callback(char* topic, byte* payload, unsigned int length) {
     Serial.println("Messagem deixada no topico: ");
     Serial.println(topic);

     //Criando a String para Armazenar o texto recebido
     Serial.print("Imprimindo a menssagem: ");
     for (int i = 0; i < length; i++) {
          Serial.print((char)payload[i]);

          //Armazenando char por char dentro da string texto
          receiver += String((char)payload[i]);
     }
     Serial.println();
     Serial.println("-----------------------");

     //imprimindo a string texto preenchida a cima recebido via mqtt
     Serial.print("String recebida: ");
     Serial.println(receiver);
     realizarAcao();
     Serial.println("-----------------------");
}

//Funcao que retorna o meu MAC
String getMacAddress() {
	uint8_t baseMac[6];
	// Get MAC address for WiFi station
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	char baseMacChr[18] = {0};
	sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
	return String(baseMacChr);
}

//Funcao para ler os valores do canal ADC e armazenar nos vetores globais para calculos posteriores.
//Cálculo Aleatorio para consumir tempo. Essa variável faz com que eu possa simular uma leitura de 3 canais ADC,
//isso serve para que eu possa simular o tempo de 17ms. É o tempo que ele demora para pegar uma proxima amostra e completar o ciclo de onda
//17 ms é o tempo que a esp32 gasta para preencher 1667 valores (1 ciclo de onda de 60hz). Porém, por questão de hardware,
//nao é possivel criar 2 vetores (tensao e corrente) de 556 valores + 1 vetor (potencia media).
//Logo para simular 1667 foi criado 2 vetores de 556 e também a leitra de um canal ADC Aleatorio
//deste modo eu simulo a leitura de 556x3 e consigo o tempo de 17ms.
//16,7 ms é um ciclo de onda! por isso 556 valores
//lixo = analogRead(33);
void lerCanaisADC(){
     //Calculando quanto tempo demora para preencher os valores no vetor de tamanho MAX
     //Tempo1 = micros();
     for (int i = 0; i < MAX; i++) {
       vetorTensao[i] = analogRead(32);
       vetorCorrente[i] = analogRead(34);
       lixo = analogRead(33);
     }
     //Tempo2 = micros();

     //Tempo para preencher os 2 vetores
     //Tempo = Tempo2-Tempo1;
     //Serial.print("Tempo para leitura dos canais ADC, e preencher os dois vetores: ");
     //Serial.print(Tempo);
     //Serial.println(" micro segundos");

     converterLeituraADC();
}

//Funcao para converter a leitura dos canais ADC em valores de Tensao e Corrente e também corrigindo a não linearidade
void converterLeituraADC(){
     //Fazendo a conversão do canal ADC para tensao e corrente. Formula = (3.3)*K/4095
     //Tempo1 = millis();
     for(int i = 0; i < MAX; i++){
       vetorTensao[i] = ((vetorTensao[i]*(3.3))/4095);//transformacao dos valores
       vetorCorrente[i] = ((vetorCorrente[i]*(3.3))/4095);//transformacao dos valores
       vetorTensao[i] = ((-0.0439)*(vetorTensao[i]*vetorTensao[i]*vetorTensao[i]))+(0.1469*(vetorTensao[i]*vetorTensao[i]))+(0.896*(vetorTensao[i]))+(0.1695); //Correção dos valores usando formula da nao linearidade
       vetorCorrente[i] = ((-0.0439)*(vetorCorrente[i]*vetorCorrente[i]*vetorCorrente[i]))+(0.1469*(vetorCorrente[i]*vetorCorrente[i]))+(0.896*(vetorCorrente[i]))+(0.1695);//Correção dos valores usando a forma da nao linearidade que esta na tabela
     }
     //Tempo2 = millis();
     //Tempo para corrigir os vetores usando a formula da não linearidade
     //Tempo = Tempo2-Tempo1;
     //Serial.print(Tempo);
     //Serial.println(" ms");
     correcaoValores();
}

void correcaoValores(){
     //Fazendo a correcao dos valores e a adição dos valores negativos
     //Tempo1 = millis();
     for (int i = 0; i < MAX; i++) {
       vetorTensao[i] = ((vetorTensao[i]-(1.62))*142.7);
       vetorCorrente[i] = ((vetorCorrente[i]-(1.62))*8);
     }
     //Tempo2 = millis();

     //Tempo para Converter os valores dos 2 vetores
     //Tempo = Tempo2-Tempo1;
     //Serial.print("Tempo conversao usando formula (3.3)*K/4095: ");
     //Serial.print(Tempo);
     //Serial.println(" ms");

     calculoPotencias();
}


//funcao para calcular potencia instantanea
void calculoPotencias(){
     //Calculando a Potencia Instantanea
     float potenciaInstantanea[MAX];
     for(int i = 0; i < MAX; i++){
       potenciaInstantanea[i] = vetorTensao[i] * vetorCorrente[i];
     }

     //Potencia Instantanea Acomulada
     float potenciaInstantaneaAcomulada = 0;
     for(int i = 0; i < MAX; i++){
            potenciaInstantaneaAcomulada = potenciaInstantaneaAcomulada + potenciaInstantanea[i];
     }

     //Calculando Potencia Media
     potenciaMedia = potenciaInstantaneaAcomulada / MAX;

     //Imprimindo a Potencia Media
     //Serial.println("Potencia Media: ");
     //Serial.println(potenciaMedia);

     /*
     //Imprimindo vetor Media
     Serial.println("Vetor de Media: \n");
     for(int i = 0; i < MAX; i++){
         Serial.print(potenciaInstantanea[i]);
         Serial.print(" - ");
     }
     Serial.println();

     //Imprimindo a o vetor de TENSAO na saida Serial
     Serial.println("Vetor de Tensao: \n");
     for(int i = 0; i < MAX; i++){
         Serial.print(vetorTensao[i]);
         Serial.print(" - ");
     }
     Serial.println();

     //Imprimindo o vetor de CORRENTE na saida Serial
     Serial.println("Vetor de Corrente: \n");
     for(int i = 0; i < MAX; i++){
         Serial.print(vetorCorrente[i]);
         Serial.print(" - ");
     }
     Serial.println();
     */
     calculoFatorDePotencia();
}

//Calculando Fator de Potencia
void calculoFatorDePotencia(){

     calculoValorEficazTensao();
     calculoValorEficazCorrente();

     //Calculando Fator de Potencia
     fatorDePotencia = (potenciaMedia/(valorEficazTensao*valorEficazCorrente));
     //Serial.printf("Fator de Potencia: ");
     //Serial.println(FatorDePotencia);
}

//Calcular valor eficaz de tensao
void calculoValorEficazTensao(){
     //Calculando Valor eficaz TENSAO
     for(int i = 0; i < MAX; i++){
          valorEficazTensao = (valorEficazTensao + (vetorTensao[i] * vetorTensao[i]));
     }
     valorEficazTensao = valorEficazTensao/MAX;
     valorEficazTensao = sqrt(valorEficazTensao);
     //Serial.print("Valor Eficaz da Tensao: ");
     //Serial.println(valorEficazTensao);
}


//Calcular valor eficaz Corrente
void calculoValorEficazCorrente(){
     //Calculando Valor eficaz CORRENTE
     for(int i = 0; i < MAX; i++){
          valorEficazCorrente = (valorEficazCorrente + (vetorCorrente[i] * vetorCorrente[i]));
     }
     valorEficazCorrente = valorEficazCorrente/MAX;
     valorEficazCorrente = sqrt(valorEficazCorrente);
     //Serial.print("Valor Eficaz da Corrente: ");
     //Serial.println(valorEficazCorrente);
}


void calculoDeMediaCircular(){
     lerCanaisADC();
     vetorMediaPotenciaMedia[contadorMediaMovel] = potenciaMedia;
     vetorMediaValorEficazTensao[contadorMediaMovel] = valorEficazTensao;
     vetorMediaValorEficazCorrente[contadorMediaMovel] = valorEficazCorrente;
     vetorMediaFatorDePotencia[contadorMediaMovel] = fatorDePotencia;
     mediaPotenciaMedia = 0;
     mediaValorEficazCorrente = 0;
     mediaValorEficazTensao = 0;
     mediaFatorDePotencia = 0;
     for(int i = 0; i < 5; i++){
          mediaPotenciaMedia = mediaPotenciaMedia + vetorMediaPotenciaMedia[i];
          mediaValorEficazCorrente = mediaValorEficazCorrente + vetorMediaValorEficazCorrente[i];
          mediaValorEficazTensao = mediaValorEficazTensao + vetorMediaValorEficazTensao[i];
          mediaFatorDePotencia = mediaFatorDePotencia + vetorMediaFatorDePotencia[i];
     }
}

void escreverServidorWeb(){

     // Check if a client has connected
     cliente  = server.available();
     if (!cliente) {
       return;
     }

     // Wait until the client sends some data
     Serial.println("New client");
     while (!cliente.available()) {
       delay(1);
     }

     //"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta charset='UTF-8'></head><body>\r\n";
     // Prepare the response
      String s = "";
      s += "HTTP/1.1 \r\nContent-Type: text/html\r\n\r\n";
      s += "<!DOCTYPE HTML><html><head><title>ESP32 Dados Servidor WEB</title><meta charset='UTF-8'></head><body>\r\n";
      s += "Informacoes Importantes: ";
      s += "<br>";
      s += "Potencia Media: ";
      s += String(mediaPotenciaMedia/5);
      s += " W<br>";
      s += "Valor Eficaz da Tensao: ";
      s += String(mediaValorEficazTensao/5);
      s += " V<br>";
      s += "Valor Eficaz de Corrente: ";
      s += String(mediaValorEficazCorrente/5);
      s += " A<br>";
      s += "Fator de Potencia: ";
      s += String(mediaFatorDePotencia/5);
      s += "<br><br><hr><br>";
      s += "Vetor de Corrente: ";
      s += "<br>";
      for(int i = 0; i < MAX; i++){
        s += " ";
        s += String(vetorCorrente[i]);
        s += " ";
      }
      s += "<br><br><hr><br>";
      s += "Vetor de tensao: ";
      s += "<br>";
      for(int i = 0; i < MAX; i++){
        s += " ";
        s += String(vetorTensao[i]);
        s += " ";
      }
      s += "</body></html>";

     // Send the response to the client
     cliente.print(s);
     delay(5000);
     cliente.stop();
}


/*


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

*/
