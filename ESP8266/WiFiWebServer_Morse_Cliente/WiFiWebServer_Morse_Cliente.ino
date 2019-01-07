#include <ESP8266WiFi.h>

const char* ssid = "AP1REUNI";
const char* password = "UeRw76=wR-";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

// Codigo Morse 1 = Longo
// Codigo Morse 0 = Curto
String Morse(char x){
  if (x == '1')
    return "01111";
  else if(x == '2')
      return "00111";
  else if(x == '3')
      return "00011";
  else if(x == '4')
      return "00001";
  else if(x == '5')
      return "00000";
  else if(x == '6')
      return "10000";
  else if(x == '7')
      return "11000";
  else if(x == '8')
      return "11100";
  else if(x == '9')
      return "11110";
  else 
    return "11111"; 
}

void PiscaLed(String Pisca){
  int i = 0;
  while(Pisca.length() > i){
    String RetornaMorse = Morse(Pisca[i]);
    for(int j = 0; j < 5; j++){
      if(RetornaMorse[j] == '0'){
        digitalWrite(2,0);
        delay(300);
        digitalWrite(2,1);
        delay(300);
      }
      else{
        digitalWrite(2,0);
        delay(1000);
        digitalWrite(2,1);
        delay(300);
      }      
    }
  i++;
  delay(3000);
  }
}

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
  s+= "<title>Morse</title>";
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
  s+= "xhttp.open('GET','morse/'+ numero.value +'/', true);";
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
    if(String("morse").equals(temp)){
      temp = strtok(NULL, "/");
      PiscaLed(temp);
      break;
    }
}

