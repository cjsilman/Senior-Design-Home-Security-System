/* TMP36 analog temperature sensor with Arduino example code. More info: https://www.makerguides.com */

// Define to which pin of the Arduino the output of the TMP36 is connected:
#include <stdio.h>
#include "espnowHelper.h"
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

#define sensorPin 33
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */
int reading[10];
RTC_DATA_ATTR int bootCount = 0;

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
  
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}
/* Function to return median */
float median(int arr[], int size)
{
    int n = size/2;
    float median= arr[n];
    return median;
}

void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
float getVoltage(){
    float busvoltage = 0;
    busvoltage = ina219.getBusVoltage_V();
    Serial.print("Voltage: "); Serial.println("busvoltage");
    return busvoltage;
  }
void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(115200);
  delay(500);
  print_wakeup_reason();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_AP_STA);
  espnowSetup();
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    }
  delay(1000);
  
  //if(sendMessageToDevice("THE TEMP SENSOR IS READY TO SEND TEMP",TMP_OK)){
 // Serial.println("Message sent");
 // }
//else{
 // Serial.println("Pairing Failed");
//}

}

void loop() {
    Serial.println("New Sample");
    Serial.println(" ");
    Serial.println(" ");
  // Get a reading from the temperature sensor:
  for(int i=0;i<10;i++){
  reading[i] = analogRead(sensorPin);
  Serial.print(reading[i]);
  Serial.print("  ");
  delay(1000);
  }
  Serial.println("array full now sorting");
  int n = sizeof(reading) / sizeof(reading[0]);
  bubbleSort(reading, n);
  Serial.println("Printing sorted Array");
  printArray(reading,n);
  Serial.println("data sorted now finding median");
  float mid= median(reading,n);
  Serial.print("median found ");
  Serial.println(mid);
  // Convert the reading into voltage:
  float voltage = mid * (3.3 / 4095);
  // Convert the voltage into the temperature in Celsius:
  float temperature_cel = (voltage - 0.35) / 0.01;
  float temperature_f =  (round ((((temperature_cel*9)/5)+32) * 10) / 10.0);
  // Print the temperature in the Serial Monitor:
  Serial.println(voltage);
  Serial.print("Temp in F is ");
  Serial.println(temperature_f);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("F");
  Serial.println("SENDING MESSAGE TO HUB");

    if(sendMessageToDevice("Current TEMP IS",TMP_OK,temperature_f)){
      Serial.println("TEMP SENT");
     }
    else{
     Serial.println("Message Failed to send");
    }
    float batVoltage = getVoltage()*1.00;
    float batteryPercent = map(batVoltage, 3.2, 4.2, 0, 100); 
    delay(500);
    if(sendMessageToDevice("Battery Voltage is :",BAT_VOL,batteryPercent)){
      Serial.println("Voltage sent");
      delay(50);
    }
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
esp_deep_sleep_start();
}
