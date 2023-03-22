#include "espnowHelper.h"

esp_now_peer_info_t peerInfo;

uint8_t hubAddr[6] = {0x44, 0x17, 0x93, 0x5F, 0xB7, 0xB0};

bool HubConnect = false;
bool HubState = false;
bool SysArmed = false;

struct_message incoming;

//MUST BE CALLED ONLY AFTER WIFI IS SETUP
void addHubToPeer() {
  memcpy(peerInfo.peer_addr, hubAddr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void espnowSetup() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_register_recv_cb(OnDataRecv);

  addHubToPeer();
}

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

//-------------------------------------
//          What you can change
//-------------------------------------

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  status == ESP_NOW_SEND_SUCCESS ? HubConnect = true : HubConnect = false;
}

//ALTER THIS FUNCTION TO MATCH YOUR CASE
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incoming, incomingData, sizeof(incoming));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Message for macAddr: ");
  for(int i = 0; i < 6; ++i)
  {
    Serial.print(incoming.macAddr[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("msg: ");
  Serial.println(incoming.msg);
  Serial.print("State: ");
  Serial.println(incoming.state);
  Serial.println();
}