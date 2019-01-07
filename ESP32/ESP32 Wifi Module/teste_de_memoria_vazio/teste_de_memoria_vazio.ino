void setup() {
    Serial.begin(115200);
    Serial.printf("------------------------------------------------------\n");
    Serial.print("Memoria livre no setup: ");
    Serial.println(ESP.getFreeHeap());
}

void loop() {
    delay(2000);
    Serial.printf("------------------------------------------------------\n");
    Serial.print("Memoria livre na main: ");
    Serial.println(ESP.getFreeHeap());

}
