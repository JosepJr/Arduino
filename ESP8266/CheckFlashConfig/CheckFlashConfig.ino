/* 
 ESP8266 CheckFlashConfig by Markus Sattler
 
 This sketch tests if the EEPROM settings of the IDE match to the Hardware
 
 */

void setup(void) {
    Serial.begin(115200);
}

void loop() {

    uint32_t realSize = ESP.getFlashChipRealSize();
    uint32_t ideSize = ESP.getFlashChipSize();
    FlashMode_t ideMode = ESP.getFlashChipMode();

    
    Serial.printf("Flash chip ID: %d\n", ESP.getFlashChipId());
    Serial.printf("Flash chip size (in bytes): %d\n", ESP.getFlashChipSize());
    Serial.printf("Flash chip speed (in Hz): %d\n", ESP.getFlashChipSpeed());   
    Serial.printf("Flash real size: %u\n\n", realSize);
    Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN")); 

    if(ideSize != realSize) {
        Serial.println("Flash Chip configuration wrong!\n");
    } else {
        Serial.println("Flash Chip configuration ok.\n");
    }

   Serial.printf("------------------------------------------------------\n");

    delay(5000);
}
