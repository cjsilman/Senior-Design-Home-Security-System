#include "espnowHelper.h"

bool messageReceived = false;

esp_now_peer_info_t peerInfo;

uint8_t hubAddr[6] = {0x44, 0x17, 0x93, 0x5F, 0xB7, 0xB0};

//Bonus WiFi to fix channel issues
constexpr char WIFI_SSID[] = "Chris's Phone";

void startWiFi() {
  WiFi.mode(WIFI_STA);

  int32_t channel = getWiFiChannel(WIFI_SSID);
  
  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after

  Serial.println("OK");
}

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}

void espnowSetup() {
  EEPROM.begin(4);

  startWifi();

  Serial.print("Mac: "); Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_register_recv_cb(OnDataRecv);
  
  addHubToPeer();

  waitForHub();
}

void addHubToPeer() {
  memcpy(peerInfo.peer_addr, hubAddr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}


bool sendMessageToDevice(const char * message, int state, float data, uint8_t* addr) {
  struct_message myData;
  strcpy(myData.msg, message);
  memcpy(myData.macAddr, addr, 6);
  myData.state = state;
  myData.data = data;
  strcpy(myData.stringMacAddr, WiFi.macAddress().c_str());
  esp_err_t result = esp_now_send(addr, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    return true;
  }
  else {
    return false;
  }
}

void waitForHub() {
  int value = EEPROM.read(3);

  if (value == 255) {
    Serial.println("Hub has already connected to this device before.");
    return;
  }

  Serial.println("Hub has not connected to this device before.");
  Serial.print("Waiting for hub");

  while(messageReceived == false)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();

  Serial.println("Writing to EEPROM");
  EEPROM.write(3, 255);
  EEPROM.commit();
  sendMessageToDevice("Startup Complete!", HUB_OK);
  messageReceived = false;
}

bool compareMacAddress(const char *mac_addr1, const char *mac_addr2)
{
  for(int i = 0; i < 17; ++i) {
    if(mac_addr1[i] == mac_addr2[i])
    {
    }
    else
    {
      return false;
    }
  }
  return true;
}
//-------------------------------------
//          What you can change
//-------------------------------------

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery Success");
  }
  else {
    Serial.println("Delivery Fail");
  }

}

//ALTER THIS FUNCTION TO MATCH YOUR CASE
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  struct_message incoming;
  memcpy(&incoming, incomingData, sizeof(incoming));

  //Checks if the message was for this device
  if(!compareMacAddress(incoming.stringMacAddr, WiFi.macAddress().c_str()))
  {
    Serial.print(incoming.stringMacAddr); Serial.print(" != "); Serial.println(WiFi.macAddress().c_str());
    return;
  }
  else
  {
    Serial.print(incoming.stringMacAddr); Serial.print(" = "); Serial.println(WiFi.macAddress().c_str());
  }

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
  Serial.print("data: ");
  Serial.println(incoming.data);
  Serial.print("State: ");
  Serial.println(incoming.state);
  Serial.print("String Mac: ");
  Serial.println(incoming.stringMacAddr);
  Serial.println();

  messageReceived = true;
}