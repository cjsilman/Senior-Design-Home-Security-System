#ifndef NODELISTFUNCTIONS_H
#define NODELISTFUNCTIONS_H

#include "node.h"
#include <vector>
#include <Firebase_ESP_Client.h>

void printNodeList(std::vector<Node> nodeList) {
    Serial.println("---Nodes in Nodelist---");
    Serial.print("Number of nodes: "); Serial.println(nodeList.size());
    for(int i = 0; i < nodeList.size(); ++i) {
      Serial.print("ID: ");
      Serial.println(nodeList[i].getID());
      Serial.print("Name: ");
      Serial.println(nodeList[i].getName());
      Serial.print("Address: ");
      nodeList[i].printMacAddr();
      Serial.print("StringAddr: ");
      Serial.println(nodeList[i].getStringMacAddr());
      Serial.print("Type: ");
      Serial.println(nodeList[i].getType());
      if(strcmp(nodeList[i].getType(), "Temperature Sensor") == 0) {
        Serial.print("Temp Range: "); 
        Serial.print(nodeList[i].getLowTemp());
        Serial.print(" - ");
        Serial.print(nodeList[i].getHighTemp());
        Serial.println(" F");
      }
        Serial.println();
    }
}

void makeNodeList(FirebaseData *fbdo , std::vector<Node> &nodeList) {
    Serial.print("Grabbing NodeList from Firebase...");

    FirebaseJson jsonNodes;
    FirebaseJsonData result;
    std::vector<String> names;
    std::vector<String> values;

    if (Firebase.ready()) {

        //Serial.println("Get Json");
        //Serial.printf("Get json... %s\n", Firebase.RTDB.getJSON(fbdo, "/nodes") ? fbdo->to<FirebaseJson>().raw() : fbdo->errorReason().c_str());

        //Firebase Json
        Firebase.RTDB.getJSON(fbdo, "/nodes");
        jsonNodes = fbdo->to<FirebaseJson>();

        jsonNodes.get(result, "/nodes");

        //Serial.println("toString pretty print");
        //jsonNodes.toString(Serial, true);

        size_t count = jsonNodes.iteratorBegin();
        for(size_t i = 0; i < count; ++i) {
        Node node;
        FirebaseJson::IteratorValue value = jsonNodes.valueAt(i);
        names.push_back(value.key);
        values.push_back(value.value);
        }
        jsonNodes.iteratorEnd();
        Serial.println("OK");
  }

    //Node Maker
    Serial.println("Creating local NodeList...");
    nodeList.clear();
    for (int i = 0; i < values.size(); i+=9) {
        Node node;

        node.setID(names[i]);
        node.setName(values[i+6]);
        node.setMacAddr(values[i+5]); //Here because it uses setName function
        node.setStringMacAddr(values[i+5]);
        node.setStatus(values[i+7].toInt());
        node.setType(values[i+8]);
        node.setTemp(values[i+4].toFloat(), values[i+3].toFloat());
        nodeList.push_back(node);
    }
    Serial.println("Local NodeList Completed");

    names.clear();
    values.clear();
}

#endif