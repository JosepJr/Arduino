#include <ESP8266WiFi.h>

const char* ssid = "PERNINHA";
const char* password = "Manicomiof4022";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
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
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Prepare the response
  String s;
  s+= "<!DOCTYPE HTML>";
  s+= "<html>";
  s+= "<head>";
  s+= " <meta charset='utf-8'/>";
  s+= "<title>OledShield</title>";
  s+= "</head>";
  s+= "<body>";
  s+= "<script type='text/javascript'>";
  s+= " function request(){";
  s+= " var numero = document.getElementById('input');";
  s+= " var xhttp = new XMLHttpRequest();";
  s+= " xhttp.onreadystatechange = function() {";
  s+= "if (this.readyState == 4 && this.status == 200) {";
  s+= "alert('Finished');";
  s+= "}";
  s+= "};";
  s+= "xhttp.open('GET','Oled/'+ numero.value +'/', true);";
  s+= "xhttp.send();";
  s+= "}";
  s+= "</script>";
  s+= "<div>";
  s+= "<input id='input' type='text'></input>";
  s+= "<button onclick='request();'>Enviar Numero</button>";
  s+= "</div>";
  s+= "</body>";
  s+= "</htlm>";

  // Send the response to the client
  client.print(s);
  client.flush();
  
  char Converte[req.length()];
  req.toCharArray(Converte, req.length());
  char *temp; //Recebe retorno no strtok
  for(temp = strtok(Converte, "/"); temp; temp = strtok(NULL,"/"))
    if(String("Oled").equals(temp)){
      temp = strtok(NULL, "/");
      PiscaLed(temp);
      break;
    }
}

