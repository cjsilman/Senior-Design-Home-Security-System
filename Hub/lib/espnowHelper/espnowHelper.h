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
#define TMP_RDY 2

#include "esp_now.h"
#include <string.h>
#include <Arduino.h>

extern uint8_t hubAddr[6];

typedef struct struct_message {
  uint8_t macAddr[6];
  char msg[32];
  int state;
  float data;
} struct_message;


//MUST BE CALLED AFTER WIFI IS SETUP
void espnowSetup(); //Call this to setup ESP now

bool sendMessageToDevice(const char * message, int state, float data = 0.0f, uint8_t* addr = hubAddr);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status); //What to do on data send

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len); //What to do on data receive


#endif