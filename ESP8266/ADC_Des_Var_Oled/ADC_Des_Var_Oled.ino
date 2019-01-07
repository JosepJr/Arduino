#include <SPI.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>
#include <math.h>
#define MAX 5 

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

MicroOLED oled(PIN_RESET, DC_JUMPER);

void setup(){    
  Serial.begin(115200);   
}

void loop(){

    oled.begin();
    oled.clear(PAGE); 
    oled.clear(ALL);
    oled.setFontType(0); 
  
   int val = 0;
   int i, j;
   double VetMedia[MAX] = {0, 0, 0, 0, 0};
   int VetAmostras[MAX] = {5, 10, 2, 3, 7};
  
   //Preenchendo o Vetor Media com a media das amostras analizadas por dia
   double Aux = 0;
   for(j = 0; j < MAX; j++){
    for(i = 0; i < VetAmostras[j]; i++){
      val = analogRead(0);
      Aux = Aux + val;
      delay(50);
    }
    VetMedia[j] = (Aux/VetAmostras[j]);
    Aux = 0;
   }

   /* Teste -------------------
   for(i = 0; i < MAX; i++){
      Serial.print(VetMedia[i], 2);
      Serial.printf(" ");    
   }
   Serial.println();
   //--------------------------*/

   //MediaAritmetica
   int MediaAritmetica =  0;
   for(i = 0; i < MAX; i++){
      MediaAritmetica = MediaAritmetica+VetMedia[i];    
   }
   MediaAritmetica = (MediaAritmetica/MAX);

   //Calculando a Variancia   
   double Variancia = 0;
   for(i = 0; i < MAX; i++){
     Variancia = (pow((VetMedia[i] - MediaAritmetica), 2) + Variancia);
   }
   Variancia = (Variancia/MAX);

   //Imprimindo no serial e no Oled
   Serial.printf("A Variancia e: ");
   Serial.print(Variancia, 2);
   Serial.println();
   
   oled.print("Variancia:");
   oled.print(Variancia, 2);
   oled.print("\n\n");
   
   //Calculando Desvio Padrao
   double DesvioPadrao = sqrt(Variancia);

   //Imprimindo no serial e no Oled
   oled.print("Desvio\nPadrao:\n");
   oled.print(DesvioPadrao, 2);
   
   Serial.printf("O Desvio padrao e: ");
   Serial.print(DesvioPadrao, 2);
   Serial.println("");
   Serial.println("");

   oled.display();
   delay(5000);   
}
