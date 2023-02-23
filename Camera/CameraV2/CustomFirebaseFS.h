#pragma once

#ifndef CustomFirebaseFS_H
#define CustomFirebaseFS_H

#define FB_ENABLE_EXTERNAL_CLIENT // define to use external client

// Use LittleFS instead of SPIFFS
#include "LittleFS.h"
#undef DEFAULT_FLASH_FS // remove Flash FS defined macro
#define DEFAULT_FLASH_FS LittleFS

// Use SD_MMC instead of SD
#if defined(ESP32)
#include <SD_MMC.h>
#undef DEFAULT_SD_FS // remove SD defined macro
#undef CARD_TYPE_SD // remove SD defined macro
#define DEFAULT_SD_FS SD_MMC
#define CARD_TYPE_SD_MMC 1
#endif

// Disable Error Queue, Firestore, FCM, Firebase Storage, Google Cloud Storage
// and Functions for Firebase.
#undef ENABLE_ERROR_QUEUE
#undef ENABLE_FIRESTORE
#undef ENABLE_FCM
#undef ENABLE_FB_STORAGE
#undef ENABLE_GC_STORAGE
#undef ENABLE_FB_FUNCTIONS



#endif