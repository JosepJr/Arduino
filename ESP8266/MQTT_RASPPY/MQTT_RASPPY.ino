//Bibliotecas
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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
int cont = 0;

//O loop mantem o ESP conectado na rede e no broker alem de ficar publicando em um topico especifico de 10 em 10 segundos...
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 2000) {
    lastMsg = now;

    cont += 1;

    client.publish(your_topic, String(cont).c_str(), true);

  }
}
