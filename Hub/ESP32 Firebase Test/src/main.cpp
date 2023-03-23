#include <Arduino.h>
#include <WiFi.h>
#include "HTTPClient.h"
#include <vector>
#include "nodelistFunctions.h"
#include <esp_now.h>
#include "espnowHelperHub.h"
#include "firebase.h"

//--------------------------------------
//              Constants
//--------------------------------------

// Network credentials
#define WIFI_SSID "lanboyp"
#define WIFI_PASSWORD "saharajoe2020.!!"

// ESPNOW Message structure
typedef struct struct_message {
  uint8_t macAddr[6];
  char msg[32];
  int state;
  float data;
  char stringMacAddr[22];
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

uint8_t broadcastAddress[] = {0x40, 0x91, 0x51, 0x1D, 0xDF, 0xD0};

int hubstatusTimer = 0;
char hubStatus[16] = {};

// Nodelist
std::vector<Node> nodeList;

// Receiving data
Node sender;
struct_message incomingReadings;
bool messageReceived;
char dataPath[32]; 
char statusPath[34]; 

//--------------------------------------
//              ESPNOW
//--------------------------------------

bool sendMessageToDevice(const char * message, int state, float data, uint8_t* addr, const char* stringMac) {
  struct_message myData;
  strcpy(myData.msg, message);
  memcpy(myData.macAddr, addr, 6);
  myData.state = state;
  strcpy(myData.stringMacAddr, stringMac);
  esp_err_t result = esp_now_send(addr, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    return true;
  }
  else {
    return false;
  }
}

// Callback Functions
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\tLast Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Successfully Received" : "Failed to Receive");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.println("--------------------------------");
  Serial.println("Message Received");
  Serial.println("--------------------------------\n");
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);

  Serial.print("From: ");
  for(int i = 0; i < 6; ++i)
  {
      Serial.print(mac[i]);
      Serial.print(" ");
  }
  Serial.println();

  Serial.print("Message for macAddr: ");
  for(int i = 0; i < 6; ++i)
  {
    Serial.print(incomingReadings.macAddr[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("msg: ");
  Serial.println(incomingReadings.msg);
  Serial.print("Data: ");
  Serial.println(incomingReadings.data);
  Serial.print("State: ");
  Serial.println(incomingReadings.state);
  Serial.print("String Mac: ");
  Serial.println(incomingReadings.stringMacAddr);
  Serial.println();

  std::vector<Node>::iterator it;
  int i = 0;
  bool nodeFound = false;

  // Search nodelist for the object that contacted us
  for(it = nodeList.begin(); it < nodeList.end(); ++it) {
    if(memcmp(mac, it->getMacAddr(), 6) == 0) {
      sender = nodeList[i];
      nodeFound = true;
      break;
    }
    ++i;
  }

  if (nodeFound == false) {
    return;
  }

  strcpy(dataPath, "nodes/");
  strcat(dataPath, sender.getID());
  strcat(dataPath, "/data");

  strcpy(statusPath, "nodes/");
  strcat(statusPath, sender.getID());
  strcat(statusPath, "/status");

  
  Serial.print("Data path: ");
  Serial.print(dataPath);
  Serial.println();

  Serial.print("Status Path: ");
  Serial.print(statusPath);
  Serial.println("\n");

  messageReceived = true;

 }


//--------------------------------------
//         Start up Functions
//--------------------------------------

void startWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  WiFi.mode(WIFI_AP_STA);

  Serial.println("OK");
}

void updateFirebaseHubInfo() {
  //Hub Address
  if (Firebase.RTDB.setString(&fbdo, "hub/hubAddress", WiFi.macAddress())) {
    Serial.print("Updated Firebase MAC Address to: ");
    Serial.println(WiFi.macAddress());
    Serial.println();
  }
  else
  {
    Serial.println("Failed to update Firebase MAC Address");
    Serial.println(fbdo.errorReason());
    Serial.println();
  }

  //Number of Nodes
  if (Firebase.RTDB.setInt(&fbdo, "hub/numberOfNodes", nodeList.size())) {
    Serial.print("Current number of active nodes: ");
    Serial.println(nodeList.size());
    Serial.println();
  }
  else
  {
    Serial.println("Failed to update number of active nodes");
    Serial.println(fbdo.errorReason());
    Serial.println();
  }
}

void startEspnow() {
// Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully initialized, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);

  esp_now_register_recv_cb(OnDataRecv);

  // Register peers
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  std::vector<Node>::iterator it;
  int i = 0;
  
  for(it = nodeList.begin(); it < nodeList.end(); ++it) {
    memcpy(peerInfo.peer_addr, it->getMacAddr(), 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
    }
  }
  
}

void attemptContactWithEachDevice() {
  std::vector<Node>::iterator it;
  int i = 0;

  for(it = nodeList.begin(); it < nodeList.end(); ++it) {
    if(it->getStatus() == -1) {
      Serial.print("Status for "); Serial.print(it->getStringMacAddr()); Serial.println(" is -1");
      if (sendMessageToDevice("This is the hub saying hello!", HUB_OK, 0.0f, it->getMacAddr(), it->getStringMacAddr()) == true) {
        Serial.println("Initial Message Sent");
      }
      else {
        Serial.print("Message failed to send to: ");
        Serial.println(it->getStringMacAddr());
        Serial.println();
      }
      delay(1000);
    }
  }
  delay(100);
}

//--------------------------------------
//              Setup
//--------------------------------------

void setup() {
  Serial.begin(115200);
  
  Serial.println("--------------------------------------");
  Serial.println("Startup");
  Serial.println("--------------------------------------");

  startWifi();

  startFirebase();

  makeNodeList(&fbdo, nodeList);

  updateFirebaseHubInfo();

  startEspnow();

  printNodeList(nodeList);

  Serial.println("--------------------------------------");
  Serial.println("Startup Complete");
  Serial.println("--------------------------------------");

  Serial.println("--------------------------------------");
  Serial.println("Verify Connection");
  Serial.println("--------------------------------------");


  attemptContactWithEachDevice();

  Serial.println("--------------------------------------");
  Serial.println("Verification Complete");
  Serial.println("--------------------------------------");
  hubstatusTimer = millis();
}

//--------------------------------------
//              Main
//--------------------------------------

void loop() {
  if (messageReceived == true) {
    Firebase.RTDB.setInt(&fbdo, statusPath, incomingReadings.state);
    Firebase.RTDB.setInt(&fbdo, dataPath, incomingReadings.data);
    messageReceived = false;
  }

  if ((millis() - hubstatusTimer) == 10000) {
    if(Firebase.RTDB.getString(&fbdo, "hub/hStatus/hubStatus")) {
      const char *str = fbdo.to<const char *>();
      strcpy(hubStatus, str);
    }
    hubstatusTimer = millis();
  }

}