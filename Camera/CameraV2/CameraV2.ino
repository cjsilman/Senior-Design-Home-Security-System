#include "esp_camera.h"
#include "soc/rtc_cntl_reg.h"
#include "CamSetup.h" 
// MicroSD Libraries
#include "FS.h"
#include "SD_MMC.h"
 
// EEPROM Library
#include "EEPROM.h"
 

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
   
  //Wake Up when Motion is Detected on Pin 13
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);
 
  Serial.println("Entering sleep mode");
  delay(1000);
  esp_deep_sleep_start();
}
 
void loop() {
 
}