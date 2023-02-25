//Camera Lib
#include "esp_camera.h"

//Brownout Error Disable Lib
#include "soc/rtc_cntl_reg.h"

//MicroSD Libs
#include "FS.h"
#include "SD_MMC.h"

//WiFi Lib
#include "WiFi.h"

//EEPROM Lib
#include "EEPROM.h"

//Custom Lib
#include "Setup.h"

bool HubConnect = false;

void setup() {
  //Turn off flash
  pinMode(GPIO_NUM_4, OUTPUT);
  digitalWrite(GPIO_NUM_4, LOW);

  //Disable Brownout Voltage Errors
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);

  while(HubConnect == false) {
     HubConnect = true;
  }

  CamConfig();
  Serial.print("Camera Configured");

  SDInit();
  Serial.print("MicroSD Initialized");

  //WifiInit();
  //Serial.print("WiFi Initialized"); 

  run();
}

void run(){

  Rapid();

  //Wake Up when Motion is Detected on Pin 13
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);
 
  Serial.println("Entering sleep mode");

  delay(1000);

  esp_deep_sleep_start();
  
}

void loop() {
  delay(1);
}
