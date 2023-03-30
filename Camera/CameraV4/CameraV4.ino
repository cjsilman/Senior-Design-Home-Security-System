//Brownout Error Disable Lib
#include "soc/rtc_cntl_reg.h"

//Wifi lib
#include "WiFi.h"

//Custom Lib
#include "Setup.h"

//Hub Communication
#include "espnowHelper.h"
#include <esp_now.h>

bool HubConnect = true;
bool SysArmed = true;



void setup() {
  
  //Disable Brownout Voltage Errors
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  //Begin Serial Comms
  Serial.begin(115200);


//Mac Address
  Serial.println("-----------------------------");
  Serial.print("Mac Address : ");
  Serial.println(WiFi.macAddress());
  Serial.println("-----------------------------");


while(HubConnect == false) {
    WiFi.mode(WIFI_AP_STA);
    espnowSetup();
    unsigned long timer = millis();
    if(sendMessageToDevice("This is the Camera!",CAM_OK)){
      HubConnect = true;

    }
    else {
      Serial.println("Message Send Fail, Sending Again.");
    }
    if(timer > 15000) {
      Serial.println("-----Failed to Connect-----");
      Serial.print("Going to Sleep");
      delay(1000);
      esp_deep_sleep_start();
    }
    
  }

/*
while(HubState == false) {
  if(incoming.state == HUB_ARMD) {
    HubState = true;
    SysArmed = true;
    Serial.println("System Armed.");
  }
  else if(incoming.state == HUB_DSARMD) {
    HubState = true;
    SysArmed = false;
    Serial.println("System Disarmed.");
  }
}
*/
delay(2000);
 
if(SysArmed == true)   {
    //sendMessageToDevice("Cam Starting Video",CAM_RDY);

    CamConfig();
    Serial.println("Camera Configured");
  
    SDInit();
    Serial.println("MicroSD Initialized");

    WifiInit();
    Serial.println("WiFi Initialized"); 

    Rapid();
  
    FirebaseUpl();
    delay(1000);
  
    Serial.println("Entering sleep mode");

    delay(1000);

    esp_deep_sleep_start();
  }

else    {
    Serial.println("System Disarmed, Entering sleep mode");

    delay(1000);

    
    esp_deep_sleep_start();
  }



//Wake if gpio pin goes high
esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);

  
}

void loop() {
  delay(1);
}
