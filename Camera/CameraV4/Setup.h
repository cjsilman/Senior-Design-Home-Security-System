#ifndef SETUP_H
#define SETUP_H

//Camera Lib
#include "esp_camera.h"

//MicroSD Libs
#include "FS.h"
#include "SD_MMC.h"

//WiFi Lib
#include "WiFi.h"

//EEPROM Lib
#include "EEPROM.h"

void disableBluetooth();
void CamConfig();
void SDInit();
void WifiInit();
void run();
void GetImage();
void Rapid();
void FirebaseUpl();

#endif