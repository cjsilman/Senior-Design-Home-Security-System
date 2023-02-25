#ifndef ESPNOW_HELPER_HUB_H
#define ESPNOW_HELPER_HUB_H

#define HUB_OK 1
#define CAM_ERR -1
#define CAM_OK 1
#define CAM_RDY 2
#define GB_ERR -1
#define GB_OK 1
#define GB_NOISE 2

#include "esp_now.h"
#include <string.h>

typedef struct struct_message {
  uint8_t macAddr[6];
  char msg[32];
  int state;
} struct_message;

bool sendMessageToDevice(const char * message, int state, uint8_t* addr) {
  struct_message myData;
  strcpy(myData.msg, message);
  memcpy(myData.macAddr, addr, 6);
  myData.state = state;
  esp_err_t result = esp_now_send(addr, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    return true;
  }
  else {
    return false;
  }
}

//Callback Functions
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  struct_message incomingReadings;
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Message for macAddr: ");
  for(int i = 0; i < 6; ++i)
  {
    Serial.print(incomingReadings.macAddr[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("msg: ");
  Serial.println(incomingReadings.msg);
  Serial.print("State: ");
  Serial.println(incomingReadings.state);
  Serial.println();
}


#endif