#include <math.h>
#define MAX 5

void setup(void)  {
  
    Serial.begin(115200);
}

void loop() {
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
   Serial.printf("A Variancia e: ");
   Serial.print(Variancia, 2);
   Serial.println();
 
   
   //Calculando Desvio Padrao
   double DesvioPadrao = sqrt(Variancia);
   Serial.printf("O Desvio padrao e: ");
   Serial.print(DesvioPadrao, 2);
   Serial.println("");
   Serial.println("");
   
   delay(5000);
}
