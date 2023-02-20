#include "CamSetup.h"
#include "esp_camera.h"
#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"
#include "EEPROM.h"
//Firebase Libs
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>


// Use 1 byte of EEPROM space
#define EEPROM_SIZE 1

//////////////////////////////////////
//Constants and Wifi Creds
//////////////////////////////////////
//EEPROM Count
unsigned int ProgCount = 0;
// Delay Between Images
unsigned int delayTime = 500;

//Wifi Creds
const char* ssid = "JD2.4";
const char* password = "Hal9cour9!";

/////////////////////////////////////
//Firebase Definitions
////////////////////////////////////
//API Key
#define API_KEY "AIzaSyCwasbPktOgE6gTIMXMlVcpc0aPidCtDgs"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "jwdurie@gmail.com"
#define USER_PASSWORD "Hal9cour9!"

//Storage Bucket  ID
#define STORAGE_BUCKET_ID "esp32-cam-photo-upload.appspot.com"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;

////////////////////////////////////
// Pin definitions for CAMERA_MODEL_AI_THINKER
////////////////////////////////////

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

 ///////////////////////////////////////////////////////////////////////////
// Configure Camera Pins and Initialize Cam
////////////////////////////////////////////////////////////////////////////

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
  config.pixel_format = PIXFORMAT_JPEG; // Choices are YUV422, GRAYSCALE, RGB565, JPEG
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
//////////////////////////////////////////////////////////////////////
// Initialize Wifi
//////////////////////////////////////////////////////////////////////

void WifiInit() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

//////////////////////////////////////////////////////////////////////
// Initialize SD Card
/////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////
//Capture and Save Image
/////////////////////////////////////////////////////////////////////

void CaptureNScan(String path) {
 
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
  
  // Return the frame buffer back to the driver for reuse
  esp_camera_fb_return(fb);
  
}

/////////////////////////////////////////////////////////////////////
//Time Lapse Image Capture
////////////////////////////////////////////////////////////////////

void TimeLapse() {

  EEPROM.begin(EEPROM_SIZE);
  ProgCount = EEPROM.read(0) + 1;
  EEPROM.write(0, ProgCount);
  EEPROM.commit();

  unsigned int pictureCount = 0;
  unsigned int ProgCount = EEPROM.read(0);

  Serial.print("EEPROM Count at = ");
  Serial.println(ProgCount);

if(ProgCount > 1) {  
  while(pictureCount < 2) {
     String path = "/image" + String(pictureCount) + ".jpg";
    Serial.printf("Picture file name: %s\n", path.c_str());
    CaptureNScan(path);
    pictureCount++;
  }
}

}
/////////////////////////////////////////////////////////////////////
//Firebase Upload
/////////////////////////////////////////////////////////////////////
void FirebaseStart() {

  WifiInit();
  SDInit();
// Assign the api key
  configF.api_key = API_KEY;
  //Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  configF.token_status_callback = tokenStatusCallback; 

  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);

  if(Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, "/image1.jpg" , mem_storage_type_sd , "/data/image1.jpg" /* path of remote file stored in the bucket */, "image/jpeg")){
    Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
  }
   else{
     Serial.println(fbdo.errorReason());
  }
   
}
