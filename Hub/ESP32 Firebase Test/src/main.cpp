#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "HTTPClient.h"
#include <vector>
#include "node.h"
#include "nodelistFunctions.h"
#include <esp_now.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

//--------------------------------------
//              Constants
//--------------------------------------

// Network credentials
#define WIFI_SSID "lanboyp"
#define WIFI_PASSWORD "saharajoe2020.!!"

// Firebase project API Key
#define API_KEY "AIzaSyAsZSHG1-VpOdG63fJtrgE_7OovbOgBmkQ"

// RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32-firebase-demo-b5b71-default-rtdb.firebaseio.com/" 

//Nodelist
std::vector<Node> nodeList;

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

uint8_t broadcastAddress[] = {0x40, 0x91, 0x51, 0x1D, 0xDF, 0xD0};

//Calback function
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



//--------------------------------------
//              Functions
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

void startFirebase() {
  Serial.print("Connecting to Firebase...");

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("OK");
}

void updateFirebase() {
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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, nodeList[4].getMacAddr(), 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

//--------------------------------------
//              Setup
//--------------------------------------

void setup() {
  Serial.begin(115200);
  
  Serial.println("-------------------");
  Serial.println("Startup");
  Serial.println("-------------------");

  startWifi();

  startFirebase();

  makeNodeList(&fbdo, nodeList);

  updateFirebase();

  startEspnow();

  printNodeList(nodeList);

  Serial.println("-------------------");
  Serial.println("Startup Complete");
  Serial.println("-------------------");

}

//--------------------------------------
//              Main
//--------------------------------------

void loop() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(nodeList[4].getMacAddr(), (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}