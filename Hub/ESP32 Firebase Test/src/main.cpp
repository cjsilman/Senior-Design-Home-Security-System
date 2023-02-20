#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "HTTPClient.h"
#include <vector>

#define MAX_NODES = 10;

//Class
class Node {
    private:
      char ID[22] = {};
      char name[32] = {};
      uint8_t macAddr[6];
      char type[32] = {};
      uint8_t charTo8bitHex(char character, char character2) {
        uint8_t temp1;
        uint8_t temp2;
        
        switch(character) {
          case '1':
            temp1 = 0x1;
            break;
          case '2':
            temp1 = 0x2;
            break;
          case '3':
            temp1 = 0x3;
            break;
          case '4':
            temp1 = 0x4;
            break;
          case '5':
            temp1 = 0x5;
            break;
          case '6':
            temp1 = 0x6;
            break;
          case '7':
            temp1 = 0x7;
            break;
          case '8':
            temp1 = 0x8;
            break;
          case '9':
            temp1 = 0x9;
            break;
          case 'A':
            temp1 = 0xA;
            break;
          case 'B':
            temp1 = 0xB;
            break;
          case 'C':
            temp1 = 0xC;
            break;
          case 'D':
            temp1 = 0xD;
            break;
          case 'E':
            temp1 = 0xE;
            break;
          case 'F':
            temp1 = 0xF;
            break;
        }

        switch(character2) {
          case '1':
            temp2 = 0x10;
            break;
          case '2':
            temp2 = 0x20;
            break;
          case '3':
            temp2 = 0x30;
            break;
          case '4':
            temp2 = 0x40;
            break;
          case '5':
            temp2 = 0x50;
            break;
          case '6':
            temp2 = 0x60;
            break;
          case '7':
            temp2 = 0x70;
            break;
          case '8':
            temp2 = 0x80;
            break;
          case '9':
            temp2 = 0x90;
            break;
          case 'A':
            temp2 = 0xA0;
            break;
          case 'B':
            temp2 = 0xB0;
            break;
          case 'C':
            temp2 = 0xC0;
            break;
          case 'D':
            temp2 = 0xD0;
            break;
          case 'E':
            temp2 = 0xE0;
            break;
          case 'F':
            temp2 = 0xF0;
            break;
        }
          
        return temp1 + temp2;

      }

    public:
      Node() = default;

      void setID(String input) {
        input.toCharArray(ID, input.length()+1);
      }

      void setName(String input){
        input.toCharArray(name, input.length()+1);
      }

      void setMacAddr(String input) {
        char temp[24] = {};
        char *token;
        uint8_t value;
        input.toCharArray(temp, input.length()+1);

        token = strtok(temp, ":\"");
        uint8_t counter = 0;

        while (token != NULL) {
          value = charTo8bitHex(token[0], token[1]);
          Serial.print(value);
          Serial.print(":");
          token=strtok(NULL, ":");
          macAddr[counter] = value;
          counter++;
        }   
        Serial.println();
      }

      void setType(String input) {
        input.toCharArray(type, input.length()+1);
      }

      char * getID() {
        return ID;
      }

      char * getName() {
        return name;
      }

      uint8_t * getMacAddr() {
        return macAddr;
      }

      void printMacAddr() {
        for(int i; i < sizeof(macAddr); ++i)
        {
          Serial.print(macAddr[i]);
          Serial.print(" ");
        }
        Serial.println();
      }

      char * getType() {
        return type;
      }
};



//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "lanboyp"
#define WIFI_PASSWORD "saharajoe2020.!!"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAsZSHG1-VpOdG63fJtrgE_7OovbOgBmkQ"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32-firebase-demo-b5b71-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

FirebaseJson jsonNodes;
FirebaseJsonArray nodesArray;
FirebaseJsonData result;
std::vector<String> names;
std::vector<String> values;
std::vector<Node> nodeList;


void loop(){ 
  if (Firebase.ready()) {

    Serial.println("Get Json");
    Serial.printf("Get json... %s\n", Firebase.RTDB.getJSON(&fbdo, "/nodes") ? fbdo.to<FirebaseJson>().raw() : fbdo.errorReason().c_str());

    //Firebase Json
    jsonNodes = fbdo.to<FirebaseJson>();

    jsonNodes.get(result, "/nodes");

    Serial.println("toString pretty print");
    jsonNodes.toString(Serial, true);

    size_t count = jsonNodes.iteratorBegin();
    Serial.println("Iterator");
    for(size_t i = 0; i < count; ++i) {
      Node node;
      FirebaseJson::IteratorValue value = jsonNodes.valueAt(i);
      names.push_back(value.key);
      values.push_back(value.value);
    }
    jsonNodes.iteratorEnd();
  }

    //Node Maker
    Serial.println("---NODE MAKER---");
    nodeList.clear();
    for (int i = 0; i < values.size(); i+=4) {
        Node node;
        node.setID(names[i]);
        node.setMacAddr(values[i+1]);
        node.setName(values[i+2]);
        node.setType(values[i+3]);
        nodeList.push_back(node);
    }

    //Node reader
    Serial.println("---NODE READER---");
    Serial.print("NodeList size: "); Serial.println(nodeList.size());
    for(int i = 0; i < nodeList.size(); ++i) {
      Serial.print("ID: ");
      Serial.println(nodeList[i].getID());
      Serial.print("Name: ");
      Serial.println(nodeList[i].getName());
      Serial.print("Address: ");
      nodeList[i].printMacAddr();
      Serial.print("Type: ");
      Serial.println(nodeList[i].getType());
      Serial.println();
    }

    names.clear();

  delay(200000);
}