//Brownout Error Disable Lib
#include "soc/rtc_cntl_reg.h"

//Wifi lib
#include "WiFi.h"

//Custom Lib
#include "Setup.h"

//Hub Communication
#include "espnowHelper.h"
#include <esp_now.h>

unsigned long start_time = millis();
unsigned long current_time;

void setup() {
  
  //Disable Brownout Voltage Errors
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  //Begin Serial Comms
  Serial.begin(115200);
  
  digitalWrite(GPIO_NUM_16,0);

/*
//Mac Address
  Serial.println("-----------------------------");
  Serial.print("Mac Address : ");
  Serial.println(WiFi.macAddress());
  Serial.println("-----------------------------");
*/

while(HubConnect == false) {
    WiFi.mode(WIFI_AP_STA);
    espnowSetup();
    current_time = millis();
    if(something about millis minus current time here) {
      if (sendMessageToDevice("This is the camera saying hello!", CAM_OK)) {
        Serial.println("Message Outgoing.");
        timer = 15000;
      }
      else 
      {
        Serial.println("Message Failed.");
        delay(1000);
      }
    }
    
    delay(1000);
  }


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


  Rapid();
  
  FirebaseUpl();
  delay(1000);
  
  
  pinMode(GPIO_NUM_4, INPUT);
  //Wake if 13 High
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);



 
if(SysArmed == true)   {

    CamConfig();
    Serial.println("Camera Configured");
  
    SDInit();
    Serial.println("MicroSD Initialized");

    WifiInit();
    Serial.println("WiFi Initialized"); 

    Rapid();
  
    FirebaseUpl();
    delay(1000);
  
    HubConnect = false;
    HubState = false;  
    Serial.println("Entering sleep mode");

    delay(1000);

    esp_deep_sleep_start();
  }

else    {
    HubConnect = false;
    HubState = false;
    esp_deep_sleep_start();
  }



//Wake if gpio pin goes high
esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);

  
}

void loop() {
  delay(1);
}
