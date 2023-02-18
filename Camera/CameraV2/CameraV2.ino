#include "esp_camera.h"
#include "soc/rtc_cntl_reg.h"
#include "CamSetup.h" 
// MicroSD Libraries
#include "FS.h"
#include "SD_MMC.h"
 
// EEPROM Library
#include "EEPROM.h"
 
// Use 1 byte of EEPROM space
#define EEPROM_SIZE 1
 
//EEPROM Count
unsigned int ProgCount = 0;


void setup() {
 
  // Disable brownout detector
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
 
  // Init Cam
  CamConfig();
  Serial.println("Camera Initialized.");
 
  // Init SD
  SDInit();
  Serial.println("MicroSD Initialized.");
  
  //Image Capture For Video Creation
  TimeLapse();

  // initialize EEPROM and Update Video Count
  EEPROM.begin(EEPROM_SIZE);
  ProgCount = EEPROM.read(0) + 1;
  EEPROM.write(0, ProgCount);
  EEPROM.commit();
 
  //Wake Up when Motion is Detected on Pin 12
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 1);
 
  Serial.println("Entering sleep mode");
  delay(1000);
  esp_deep_sleep_start();
 
}
 
void loop() {
 delay(1000);
}