#include <WiFi.h>
#include <esp_wifi.h>
#include "espnowHelper.h"

void setup() {
  Serial.begin(115200);
  espnowSetup();
}

void loop() {
  float temp = random(1,90);
  sendMessageToDevice("Test Message to hub", TMP_OK, temp);
  delay(10000);
}
