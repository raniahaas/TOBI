/**
05/22/2026 - RH - Basework for multithreading integration
06/02/2026 - RH - Fixes to the RTOS declarations 
06/14/2026 - RH - Added Startup sequence for IMU
06/14/2026 - RH - Battery Voltage display for serial log
06/16/2026 - RH - Testing for IMU; without battery testing which will need to be done independently 
                - Moved Startup function to separate file; tested battery with computer not LiPO
*/

#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <MS5611.h>
#include "startup/startupSeq.h"
#include <SimpleBatteryMonitor.h>

// Note that the GPIO number is offset by one so D2 is actually GPIO3 instead of GPIO2

// #define LED1_PIN 3 // 2 + 1
// #define LED2_PIN 5 // 4 + 1

//Sensors
// Adafruit_LSM6DSO32 dso32;
// MS5611 baro(0x77);


//Tasks
TaskHandle_t TaskStart;
TaskHandle_t TaskFileLogging;
TaskHandle_t TaskEventLogging;
TaskHandle_t TaskUserTests;
TaskHandle_t TaskShutdown;
TaskHandle_t TaskIMU;
TaskHandle_t TaskBaro;

const int led1 = 3;
const int led2 = 5; //possibly may need to change


void imuWrite(void *pvParameters)  {
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void readBarometer(void *pvParameters){
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void writeEvents(void *pvParameters){
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void fileLogging(void *pvParameters){
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void shutDown(void *pvParameters){
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void userTests(void *pvParameters){
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200); //default baud rate
  delay(3000); //Longer delay time here required to just see the Serial Monitor information

  Wire.begin(5,6);

  runStartupSequence();
 
  delay(2000);
  
  

  /*
  Core 0:
    Used for all event logging
      File logging
      Events
      User tests
  Core 1:
    All fetching from instruments
      IMU
      Accel/Baro
  */

  //Core 0 Tasks                 
  xTaskCreatePinnedToCore(
    writeEvents,   
    "eventLogging",   
    4096,     
    NULL,      
    2,          
    &TaskEventLogging,   
    0);        

  xTaskCreatePinnedToCore(
    fileLogging,   
    "fileLogging",   
    4096,     
    NULL,      
    1,          
    &TaskFileLogging,   
    0);        

  xTaskCreatePinnedToCore(
    userTests,   
    "TestMode",   
    4096,     
    NULL,      
    1,          
    &TaskUserTests,   
    0);        

  xTaskCreatePinnedToCore(
    shutDown,   
    "PoweringOff",   
    4096,     
    NULL,      
    1,          
    &TaskShutdown,   
    0);        

  //Core 1 Tasks
  xTaskCreatePinnedToCore(
    imuWrite,   
    "imu",   
    4096,     
    NULL,      
    1,          
    &TaskIMU,   
    1);        

    xTaskCreatePinnedToCore(
    readBarometer,   
    "barometer",   
    4096,     
    NULL,      
    1,          
    &TaskBaro,   
    1);        
}


void loop() {
  vTaskDelete(NULL);
}

