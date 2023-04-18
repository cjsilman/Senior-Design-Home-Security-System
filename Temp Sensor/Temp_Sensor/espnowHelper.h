#ifndef ESPNOW_HELPER_H
#define ESPNOW_HELPER_H

#define HUB_OK 1
#define HUB_ARM 2
#define HUB_DISARM 3
#define CAM_ERR -1
#define CAM_OK 1
#define CAM_RDY 2
#define GB_ERR -1
#define GB_OK 1
#define GB_NOISE 2
#define TMP_ERR -1
#define TMP_OK 1
#define TMP_ALRT 2
#define GB_ERR -1
#define ENT_OK 1
#define ENT_DET 2
#define BAT_VOL 5

#include <string.h>
#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <EEPROM.h>
#include <WiFi.h>

extern bool messageReceived;

extern uint8_t hubAddr[6];

typedef struct struct_message {
  uint8_t macAddr[6];
  char msg[32];
  int state;
  float data;
  char stringMacAddr[22];
} struct_message;



void startWiFi();

int32_t getWiFiChannel(const char* ssid);

//MUST BE CALLED AFTER WIFI IS SETUP
void espnowSetup(); //Call this to setup ESP now

void addHubToPeer();

bool sendMessageToDevice(const char * message, int state, float data = 0.0f, uint8_t* addr = hubAddr);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status); //What to do on data send

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len); //What to do on data receive

void waitForHub();

bool compareMacAddress(const char *mac_addr1, const char *mac_addr2);

#endif