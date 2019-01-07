#define MAX 800

void verifica(){
  while (true){
    if(digitalRead(4) == LOW){
      break;
    }
  }  
}

void setup(void) {

  pinMode(2, OUTPUT);
  pinMode(4, INPUT);
  Serial.begin(115200);
}

void loop() {

    digitalWrite(2, HIGH);
    int vector[MAX];
    //Calculando quanto tempo demora para preencher os valores no vetor de tamanho MAX
    int t1 = millis();
    for (int i = 0; i < MAX; i++) {
      vector[i] = analogRead(0);
    }
    int t2 = millis();
    Serial.println();
    Serial.println("-------------Tempo---------");
    Serial.println(t1);
    Serial.println(t2);
    Serial.println(t2 - t1);
    Serial.println("---------------------------");
    Serial.println("-------FIM DE CODIGO-------");   
    verifica();
    
  delay(1000);
}



/*Exibindo em ADC
      int val = analogRead(0);
      Serial.printf("%d ADC\n", val);
      Serial.println();

      //Exibindo em Volt
      double  x = ((val*(3.3))/1024);

      Serial.print(x,2);
      Serial.println(" Volt's\n");
      Serial.println();*/

    /*Serial.println();
      for (int i = 0; i < MAX; i++){
        //Serial.println();
        //Serial.print(i);

        Serial.printf("%d: ",i);
        Serial.printf("%d \n",vector[i]);
      }*/
