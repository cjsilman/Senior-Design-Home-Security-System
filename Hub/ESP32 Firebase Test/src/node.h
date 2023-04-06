#ifndef NODE_H
#define NODE_H
#include <Arduino.h>

class Node {
    private:
      char ID[22] = {};
      char name[32] = {};
      uint8_t macAddr[6];
      char stringMacAddr[22] = {};
      char type[32] = {};
      int status;
      float lowTemp;
      float highTemp;

      uint8_t charTo8bitHex(char character, char character2);

    public:
      Node() = default;

      void setID(String input);

      void setName(String input);

      void setMacAddr(String input);

      void setStringMacAddr(String input);

      void setType(String input);

      void setStatus(int status);

      void setTemp(float LTemp, float HTemp);

      char * getID();

      char * getName();

      uint8_t * getMacAddr();

      char * getStringMacAddr();

      char * getType();

      int getStatus();

      void printMacAddr();

      float getLowTemp();

      float getHighTemp();
};
#endif