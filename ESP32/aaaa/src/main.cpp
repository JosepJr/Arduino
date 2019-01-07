#include <Arduino.h>
#define MAX 10

void setup() {
    Serial.begin(115200);
    // put your setup code here, to run once:
}

void loop() {
  float Canal32;
  float media = 0;
  for(int i = 0; i < MAX; i++){
    Canal32 = ((analogRead(32)*(3.3))/4095);
    media = media + Canal32;
  }
  delay(2000);
  media = media / MAX;
  Serial.println(media);
}
