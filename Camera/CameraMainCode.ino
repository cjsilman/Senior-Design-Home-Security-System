#include <WiFi.h>
#include "esp_camera.h"
#include "esp_http_server.h"
#include "ServerStart.h"


void setup() { 
  setupServer();
}

void loop() {

esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1);

Serial.print('\n');
Serial.print("MOTION DETECTED, PANIC!!!!!");
startCameraServer();
delay(60000);

int motion = digitalRead(GPIO_NUM_13);

if(motion == LOW) {
    Serial.print('\n');
    Serial.print("Bedtime");
    esp_deep_sleep_start();

  }  
 delay(10);
}