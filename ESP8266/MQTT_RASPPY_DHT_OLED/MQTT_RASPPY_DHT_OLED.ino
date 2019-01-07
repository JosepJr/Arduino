//Bibliotecas
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "DHT.h"
#define DHTPIN 2     // Porta
#define DHTTYPE DHT22
#include <SPI.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

DHT dht(DHTPIN, DHTTYPE);
MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C declaration

#define wifi_ssid "Fisica"
#define wifi_password "Fisic@42"

#define mqtt_server "192.168.0.101"
#define mqtt_user ""
#define mqtt_password ""

#define your_topic "/bme"

WiFiClient espClient;
PubSubClient client(espClient);

//Inicia a serial e conecta o ESP na rede
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

//Função que conecta o ESP na rede
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Funcao que reconecta o ESP no Broker da rede
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//Variaveis globais
long lastMsg = 0;

//O loop mantem o ESP conectado na rede e no broker alem de ficar publicando em um topico especifico de 10 em 10 segundos...
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(100);
  dht.begin();
  oled.begin();
  double h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  double t = dht.readTemperature();


  oled.clear(PAGE); //clear will clear the Arduino's display buffer.
  oled.clear(ALL); // Clear the display's memory (gets rid of artifacts)
  oled.setFontType(0);  // Set the text to small (10 columns, 6 rows worth of characters).

  oled.print("Humidity: ");
  oled.print(h, 2);
  oled.print(" %\n\n");
  oled.print("Temp:\n");
  oled.print(t, 2);
  oled.print(" *C");

  Serial.println();
  Serial.print("Umidade: ");
  Serial.print(h, 2);
  Serial.print(" %\n\n");
  Serial.print("Temperatura: ");
  Serial.print(t, 2);
  Serial.println(" *C");

  oled.display();

  client.publish(your_topic, String(h).c_str(), true);
  client.publish(your_topic, String(t).c_str(), true);
  delay(1900);
}
