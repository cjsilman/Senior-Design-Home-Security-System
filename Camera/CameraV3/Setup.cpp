#include <Arduino.h>

//Camera Lib
#include "esp_camera.h"

//Brownout Error Disable Lib
#include "soc/rtc_cntl_reg.h"

//MicroSD Libs
#include "FS.h"
#include "SD_MMC.h"

//WiFi Lib
#include "WiFi.h"

//EEPROM Lib
#include "EEPROM.h"

//-------------------------------------------------------
//Define Usage of 1 Byte of EEPROM Space

#define EEPROM_SIZE 1


//-------------------------------------------------------
//SSID and Password for Wifi

const char* ssid = "";
const char* password ="";


//-------------------------------------------------------
//Pin Definitions For Cam

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

//-------------------------------------------------------
//Variable Definitions

//Image Aquisition
unsigned int delayTime = 200; //Delay Between Image Capture
unsigned int picNum = 60; //Number of Photos to be Captured
unsigned int fps = 1000/delayTime; //Frames Captured per second
unsigned int vidlength = delayTime * picNum; //ms calculation of recording length

//EEPROM Count 
unsigned int ProgCount = 0;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Camera Configuration Function

void CamConfig() {
   

  camera_config_t config;
 
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; //YUV422, GRAYSCALE, RGB565, JPEG
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10; //10-63 
  config.fb_count = 5;
  
  // Init Cam
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  } 
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Initialize MicroSD Card

void SDInit() {
 
  Serial.println("Mounting MicroSD Card");
  if (!SD_MMC.begin()) {
    Serial.println("MicroSD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No MicroSD Card found");
    return;
  }
 
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Initialize WiFi

 void WifiInit() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Capture Frame from Cam Module

void GetImage(String path) {

  // Setup frame buffer
  camera_fb_t  * fb = esp_camera_fb_get();
 
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
 
  // Save picture to microSD card
  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file in write mode");
  }
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path.c_str());
  }
  file.close();

 // Return the frame buffer 
  esp_camera_fb_return(fb);
  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Rapid Image Aquisition Loop

void Rapid() {
  
  EEPROM.begin(EEPROM_SIZE);
  ProgCount = EEPROM.read(0) + 1;
  EEPROM.write(0, ProgCount);
  EEPROM.commit();

  ProgCount = EEPROM.read(0);

  Serial.print("EEPROM Count at = ");
  Serial.println(ProgCount);
  
unsigned int pictureCount = 0;
  
if(ProgCount > 1) {  
  Serial.printf("Capturing", picNum, "Images at", fps, "Frames Per Second");
  Serial.println();
  while(pictureCount < picNum) {
     String path = "/image" + String(pictureCount) + ".jpg";
    Serial.printf("Picture file name: %s\n", path.c_str());
    GetImage(path);
    ++pictureCount;
    delay(delayTime);
  }
}

}


