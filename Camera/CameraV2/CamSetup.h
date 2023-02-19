#ifndef CAMSETUP_H
#define CAMSETUP_H

#include "esp_camera.h"
#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"

void CamConfig();
void SDInit();
void CaptureNScan();
void TimeLapse();

#endif