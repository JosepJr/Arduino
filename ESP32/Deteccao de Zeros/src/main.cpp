#include <Arduino.h>

unsigned long Tempo;
unsigned long Tempo1;
unsigned long Tempo2;

void setup() {
    Serial.begin(115200);
}

void loop() {
     float vetorTensao[10];
     float vetorCorrente[10];
     float media1 = 0;
     float media2 = 0;
     
     do{
          for (int i = 0; i < 10 ; i++) {
               vetorTensao[i] = analogRead(32);
          }

     }while(media1 < 2048 && media2 > 0);


}
