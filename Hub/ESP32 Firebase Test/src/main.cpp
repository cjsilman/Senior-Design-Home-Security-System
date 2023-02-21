#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "HTTPClient.h"
#include <vector>
#include "node.h"
#include "nodelistFunctions.h"

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

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

void startWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
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

void setup() {
  Serial.begin(115200);
  
  Serial.println("-------------------");
  Serial.println("Startup");
  Serial.println("-------------------");

  startWifi();

  startFirebase();

  makeNodeList(&fbdo, nodeList);

  updateFirebase();

  printNodeList(nodeList);

  Serial.println("-------------------");
  Serial.println("Startup Complete");
  Serial.println("-------------------");

}

void loop() {

}