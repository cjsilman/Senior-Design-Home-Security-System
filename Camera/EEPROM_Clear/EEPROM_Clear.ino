#include "esp_camera.h"
#include "EEPROM.h"
#define EEPROM_SIZE 1


void setup() {
  
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  unsigned int EEPROM_init = EEPROM.read(0);

  EEPROM.write(0, 0);
  
  EEPROM.commit();

  Serial.print("EEPROM Reset to 0 from ");
  Serial.print(EEPROM_init);
  
  
  delay(500);
  
}

void loop() {
  

}
