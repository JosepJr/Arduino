
//Bibliotecas
#include <esp_wifi.h>
#include <WiFi.h>
#include <PubSubClient.h>

//mosquitto_sub -h [ip] -p [porta] -u[usuario] -P [senha] -t [local do post /algumacoisa/etc]
//mosquitto_pub -h [ip] -p [porta] -u[usuario] -P [senha] -t [local do post /algumacoisa/etc] -m [mensagem]

#define wifi_ssid "Fisica"
#define wifi_password "Fisic@42"

#define mqtt_server "192.168.0.49"
#define mqtt_port 1883
#define mqtt_user "tccjosep"
#define mqtt_password "tcc"

//publicando qualquer coisa no canal /pub
#define publicar "/pub"
//lendo qualquer coisa do canal /sub
#define inscrever "/sub"

WiFiClient espClient;
PubSubClient client(espClient);

//função para ser subscribe do topico definido, ou seja, vou ficar ouvindo tudo que tiver no topico.
void callback(char* topic, byte* payload, unsigned int lenght){
  Serial.print("Mensagem deixada no topico: ");
  Serial.println(topic);
  Serial.print("Menssagem:");
  for(int i = 0; i < lenght; i++){
      Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("----------------------------");
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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//Funcao que reconecta o ESP no Broker da rede
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32 Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(2000);
    }
  }
}

//Inicia a serial e conecta o ESP na rede e no servidor mqtt
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  randomSeed(analogRead(0));

  if (!client.connected()) {
    reconnect();
  }
}

//O loop mantem o ESP conectado na rede e no broker alem de ficar publicando
//em um topico especifico de 10 em 10 segundos...
void loop() {
  //publicando mensagem no topico
  int x = random(99999);
  client.publish(publicar, String(x).c_str(), true);
  Serial.println(x);
  delay(3000);

  //lendo mensagem do tópico
  client.setCallback(callback);
  client.subscribe(inscrever);

 client.loop();
}
