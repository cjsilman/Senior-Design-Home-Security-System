#include "node.h"

//Public Functions
void Node::setID(String input) {
    input.toCharArray(ID, input.length()+1);
}

void Node::setName(String input){
    input.remove(0,1);
    input.remove(input.length()-1,1);
    input.toCharArray(name, input.length()+1);
}

void Node::setStringMacAddr(String input){
    input.remove(0,1);
    input.remove(input.length()-1,1);
    input.toCharArray(stringMacAddr, input.length()+1);
}

void Node::setType(String input) {
    input.remove(0,1);
    input.remove(input.length()-1,1);
    input.toCharArray(type, input.length()+1);
} 

char * Node::getID() {
    return ID;
}

char * Node::getName() {
    return name;
}

uint8_t * Node::getMacAddr() {
    return macAddr;
}

char * Node::getStringMacAddr() {
    return stringMacAddr;
}

void Node::printMacAddr() {
    for(int i; i < 6; ++i)
    {
        Serial.print(macAddr[i]);
        Serial.print(" ");
    }
    Serial.println();
}

char * Node::getType() {
    return type;
}

void Node::setMacAddr(String input) {
        
    //Create addr variable to temporarily store address
    uint8_t addr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    
    // String comes with "" around it, remove those
    input.remove(0,1);
    input.remove(input.length()-1,1);

    // If not the correct size, omit and output error
    if (input.length() != 17) {
        Serial.print("ERR: Incorrect address with name: ");
        Serial.println(getName());
        Serial.print("Assigning default: ");
        memcpy(macAddr, addr, 6);
        printMacAddr();
        return;
    }

    //These are used for conversion from string to char
    char temp[24] = {};
    char *token;

    //This is used to convert FF -> 0xFF
    uint8_t value;
    
    input.toCharArray(temp, input.length()+1);

    token = strtok(temp, ":\"");
    uint8_t counter = 0;

    while (token != NULL) {
        value = charTo8bitHex(token[1], token[0]);
        token=strtok(NULL, ":");
        addr[counter] = value;
        counter++;
    }   
    memcpy(macAddr, addr, 6);
}

//Private Helper Function
uint8_t Node::charTo8bitHex(char character, char character2) {
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
          default:
            temp1 = 0x00;
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
          default:
            temp2 = 0x00;
            break;
        }
          
        return temp1 + temp2;
}