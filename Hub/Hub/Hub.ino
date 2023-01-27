// This program defines the functionality of our hub.
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SD.h"
#include "SPI.h"

// Pin Definitions
#define SD_CHIPSELECT 4

// WiFi Network Credentials
const char* wifi_network_ssid = "lanboyp";
const char* wifi_network_password = "saharajoe2020.!!";

//Soft Access Point
const char *soft_ap_ssid = "ESP32";
const char *soft_ap_password = "testpassword";

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

void initSDCard() {
  //Checks if SD is on CS pin
  if(!SD.begin(SD_CHIPSELECT)) {
    Serial.println("Card Mount Failed");
  }

  //Determines card type
  uint8_t cardType = SD.cardType();
  switch(cardType) {
    case CARD_NONE:
      Serial.println("No SD card attached");
      break;
    case CARD_MMC:
      Serial.println("MMC");
      break;
    case CARD_SD:
      Serial.println("SDSC");
      break;
    case CARD_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }

  //Print card size
  uint64_t cardSize = SD.cardSize() / (1024*1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void initWiFi() {
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.begin(wifi_network_ssid, wifi_network_password);
}

void initSoftAP() {
  WiFi.softAP(soft_ap_ssid, soft_ap_password);
}

void setup() {
  Serial.begin(115200);
  initSDCard();
  initWiFi();
  initSoftAP();
}

void loop() {
  // put your main code here, to run repeatedly:

}
