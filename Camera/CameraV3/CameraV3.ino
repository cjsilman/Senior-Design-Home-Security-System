//Brownout Error Disable Lib
#include "soc/rtc_cntl_reg.h"

//Custom Lib
#include "Setup.h"

bool HubConnect = false;

void setup() {
  
  //Disable Brownout Voltage Errors
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  //Begin Serial Comms
  Serial.begin(115200);

  while(HubConnect == false) {
     HubConnect = true;
  }

  CamConfig();
  Serial.println("Camera Configured");
  
  SDInit();
  Serial.println("MicroSD Initialized");

  WifiInit();
  Serial.println("WiFi Initialized"); 

  Rapid();
  
  FirebaseUpl();
  delay(1000);

  //Wake if 13 High
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_16, 1);
 
  Serial.println("Entering sleep mode");

  delay(1000);

  esp_deep_sleep_start();
  
}

void loop() {
  delay(1);
}
