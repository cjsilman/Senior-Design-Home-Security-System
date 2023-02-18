#ifndef SERVERSTART_H
#define SERVERSTART_H

#include <WiFi.h>
#include "esp_camera.h"
#include "esp_http_server.h"
#include <Arduino.h>

void startCameraServer();
void setupServer();



#endif