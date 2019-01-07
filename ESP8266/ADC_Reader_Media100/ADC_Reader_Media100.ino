void setup(void) {
  
    Serial.begin(115200);
}

void loop() {
   int val = 0;
   int i;
   int media = 0;
   
   for(i = 0; i < 100; i++){
    val = analogRead(0);
    media = (media+val);
    delay(50);   
   }
  
   media = (media/100);
   
   //Exibindo em ADC
   Serial.printf("Media de 100 valore: %d ADC\n", media);  
   Serial.println();

   //Exibindo em Volt 
   double x = ((media*(3.3))/1023); 

   Serial.printf("Media de 100 valores: ");
   Serial.print(x,2);
   Serial.println(" Volt's\n");
   Serial.println();

    delay(1000);
}
