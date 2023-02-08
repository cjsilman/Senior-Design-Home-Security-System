#ifndef HubManager_H
#define HubManager_H

#define MAX_SIZE 6

#include "Arduino.h"

class HubManager {
  private:
    uint8_t hubAddr[MAX_SIZE];
  
  public:
    HubManager(uint8_t* addr);
    void printAddr();

};

#endif