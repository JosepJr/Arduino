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
  pinMode(5, OUTPUT);
  digitalWrite(5, 0);
  
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
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<head><meta charset='utf-8'/><title>Controlando Reles</title></head><body>";
  s += "<script type='text/javascript'>";
  s += "function request(numero){";
  s += "if(numero == 1){";
  s += " var url = 'ligar';";
  s += "}";
  s += "else{";
  s += " var url = 'desligar';}";
  s += " var xhttp = new XMLHttpRequest();";
  s += " xhttp.onreadystatechange = function(){";
  s += "     if (this.readyState == 4 && this.status == 200){";
  s += "      alert('Finished');";
  s += "    }";
  s += " };";
  s += "    xhttp.open('GET',url,true);";
  s += "  xhttp.send();";
  s += " }";
  s += " </script>";
  s += "<div>";
  s += "<button onclick='request(1);'>Ligar</button>";
  s += "<button onclick='request(0);'>Desligar</button>";
  s += "</div>";
  s += "</body>";
  s += "</htlm>";


  // Send the response to the client
  client.print(s);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/ligar") != -1)
    val = 0;
  else if (req.indexOf("/desligar") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    //client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(5, val);
  
  client.flush();
  delay(1);
  Serial.println("Client disconnected");

}

