#include <WiFi.h>
#include <PubSubClient.h>

//mosquitto_sub -h [ip] -p [porta] -u[usuario] -P [senha] -t [local do post /algumacoisa/etc]
//mosquitto_pub -h [ip] -p [porta] -u[usuario] -P [senha] -t [local do post /algumacoisa/etc] -m [mensagem]

/*const char* ssid = "Fisica";
const char* password = "Fisic@42";
const char* mqttServer = "192.168.0.49";
*/

const char* ssid = "Republica Virus";
const char* password = "Intelcore";
const char* mqttServer = "192.168.2.22";


/*const char* ssid = "AP1REUNI";
const char* password = "Fisic@42";
const char* mqttServer = "192.168.0.49";
*/

const int mqttPort = 1883;
const char* mqttUser = "tccjosep";
const char* mqttPassword = "tcc";

WiFiClient espClient;
PubSubClient client(espClient);

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
     Serial.println(texto);
     Serial.println("-----------------------");
     }

void setup() {

     Serial.begin(115200);
     WiFi.begin(ssid, password);

     while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.println("Connecting to WiFi..");
     }
     Serial.println("Connected to the WiFi network");

     client.setServer(mqttServer, mqttPort);
     client.setCallback(callback);

     while (!client.connected()) {
          Serial.println("Connecting to MQTT...");

          if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
               Serial.println("connected");
          }
          else {
                Serial.print("failed with state ");
                Serial.print(client.state());
                delay(2000);

          }
     }
     client.subscribe("/pub");
}

void loop() {
     while (!client.connected()) {
          Serial.println("Connecting to MQTT...");

          if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
               Serial.println("connected");
               client.subscribe("/pub");
          }
          else {
                Serial.print("failed with state ");
                Serial.print(client.state());
                delay(2000);

          }
     }
     int x = random(99999);
     client.publish("/sub", String(x).c_str(), true);
     Serial.println(x);
     delay(3000);
     client.loop();
}
