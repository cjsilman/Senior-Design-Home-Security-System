#include "HubManager.h"

HubManager::HubManager(uint8_t* addr) {
	memcpy(hubAddr, addr, MAX_SIZE);
}

void HubManager::printAddr() {
	for (int i = 0; i < MAX_SIZE; ++i)
	Serial.println(hubAddr[i]);
}