/**
05/22/2026 - RH - Basework for multithreading integration
06/02/2026 - RH - Fixes to the RTOS declarations 
*/

#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <MS5611.h>
#include "startup/imu_init.h"
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


//Code section for tasks
void startUp(void *pvParameters){
  /*Things to check on startup:
    Continuity in batteries
    IMU check
    Accelerometer Check
    Barometer Check

  */
  Serial.println("Starting device. Commencing Start up sequence.");
  
  if (!initIMU()){
    Serial.println("IMU Startup Failed. Retry device.");
    vTaskDelete(NULL);
  } else {
    Serial.println("IMU in check, all systems running. Continuing boot process...");
    vTaskDelete(NULL);
  }


}

void imuWrite(void *pvParameters)  {

}

void readBarometer(void *pvParameters){

}

void writeEvents(void *pvParameters){

}

void fileLogging(void *pvParameters){

}

void shutDown(void *pvParameters){

}

void userTests(void *pvParameters){

}

void setup() {
  Serial.begin(115200); //default baud rate
  delay(500);

  /*
  Core 0:
    Used for all event logging
      Startup and Shutdown
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
    startUp, 
    "Starting",   
    10000,     //stack size, will want to change later into development 
    NULL,      
    1,      //additionally may want to consider changing priorites   
    &TaskStart,     
    0);                   

  xTaskCreatePinnedToCore(
    writeEvents,   
    "eventLogging",   
    10000,     
    NULL,      
    1,          
    &TaskEventLogging,   
    1);        

  xTaskCreatePinnedToCore(
    fileLogging,   
    "fileLogging",   
    10000,     
    NULL,      
    1,          
    &TaskFileLogging,   
    1);        

  xTaskCreatePinnedToCore(
    userTests,   
    "TestMode",   
    10000,     
    NULL,      
    1,          
    &TaskUserTests,   
    1);        

  xTaskCreatePinnedToCore(
    shutDown,   
    "PoweringOff",   
    10000,     
    NULL,      
    1,          
    &TaskIMU,   
    1);        

  //Core 1 Tasks
  xTaskCreatePinnedToCore(
    imuWrite,   
    "imu",   
    10000,     
    NULL,      
    1,          
    &TaskIMU,   
    1);        

    xTaskCreatePinnedToCore(
    readBarometer,   
    "barometer",   
    10000,     
    NULL,      
    1,          
    &TaskBaro,   
    1);        
}


void loop() {

}

