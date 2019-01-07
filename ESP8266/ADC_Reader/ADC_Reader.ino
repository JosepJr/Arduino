  void setup(void) {
  
    Serial.begin(115200);
}

void loop() {

   int val = 0;
   val = analogRead(0);
   
   //Exibindo em ADC
   Serial.printf("%d ADC\n", val);  
   Serial.println();

   //Exibindo em Volt
   double  x = ((val*(3.3))/1024);

   Serial.print(x,2);
   Serial.println(" Volt's\n");
   Serial.println();

   delay(1000);
}
