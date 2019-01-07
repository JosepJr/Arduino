/*
Como configurar o ESP32 para usar mqtt
mosquitto_sub -h [ip] -p [porta] -u[usuario] -P [senha] -t [local do post /algumacoisa/etc]
mosquitto_pub -h [ip] -p [porta] -u[usuario] -P [senha] -t [local do post /algumacoisa/etc] -m [mensagem]
*/

#include <WiFi.h>
#include <PubSubClient.h>


//Redes WIFI

const char* ssid = "Fisica";
const char* password = "Fisic@42";
const char* mqttServer = "192.168.0.6";


//const char* ssid = "Republica Virus";
//const char* password = "Intelcore";
//const char* mqttServer = "192.168.2.22";

/*const char* ssid = "AP1REUNI";
const char* password = "";
const char* mqttServer = "192.168.0.49";
*/

const int mqttPort = 1883;
const char* mqttUser = "tccjosep";
const char* mqttPassword = "tcc";
const char* publicar = "/publisher";
const char* inscrever = "/subscriber";

WiFiClient espClient;
PubSubClient client(espClient);


//Funções

//Realizar acção de acordo com a string recebida do canal de inscricao do MQTT
void realizarAcao(String texto){
     if(texto.equalsIgnoreCase("PIN05ON")){
          digitalWrite(5, LOW);
          client.publish(publicar, String("Led Ligado").c_str(), true);
     }
     if(texto.equalsIgnoreCase("PIN05OFF")){
          digitalWrite(5, HIGH);
          client.publish(publicar, String("Led Desligado").c_str(), true);
     }
}

//Publicando no servidor designado
void publicarServidor(){
     int x = random(99999);
     client.publish(publicar, String(x).c_str(), true);
     Serial.println(x);
}

//Função para pegar no topico definido a string e Armazenar em uma variável
void callback(char* topic, byte* payload, unsigned int length) {
     Serial.println("Messagem deixada no topico: ");
     Serial.println(topic);

     //Criando a String para Armazenar o texto recebido
     String texto;

     Serial.print("Imprimindo a menssagem: ");
     for (int i = 0; i < length; i++) {
          Serial.print((char)payload[i]);

          //Armazenando char por char dentro da string texto
          texto += String((char)payload[i]);
     }
     Serial.println();
     Serial.println("-----------------------");

     //imprimindo a string texto preenchida a cima recebido via mqtt
     Serial.print("String recebida: ");
     Serial.println(texto);
     realizarAcao(texto);
     Serial.println("-----------------------");
}

void conectarRedeWiFi(){
     if(WiFi.status() != WL_CONNECTED){
          Serial.printf("Conectando na rede WiFi %s \n", ssid);
          while (WiFi.status() != WL_CONNECTED) {
               delay(500);
               Serial.print(".");
          }
          Serial.printf("\nConexão com a rede estabelecida...\n");
     }
}

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

void setup(){
     Serial.begin(115200);
     WiFi.begin(ssid, password);

     pinMode(5, OUTPUT);

     conectarRedeWiFi();

     client.setServer(mqttServer, mqttPort);
     client.setCallback(callback);

}

void loop(){

     client.loop();

     conectarRedeWiFi();
     conectarServidorMQTT();
     publicarServidor();

     delay(3000);

     client.loop();

}
