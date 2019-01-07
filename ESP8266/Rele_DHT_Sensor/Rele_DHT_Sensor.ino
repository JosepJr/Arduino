#include "DHT.h"

#define DHTPIN 2     // Porta


//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  Serial.begin(115200);

  //prepare GPIO2
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);  
} 


void loop(){
  
  dht.begin();  
  
  double h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  double t = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("\n");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("\n");
  

  if(h < 60){
    digitalWrite(5, LOW);
  }
  
  if(h >= 60){
    digitalWrite(5, HIGH);    
  }
  
  delay(2000);
}

