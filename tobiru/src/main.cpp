/**
05/22/2026 - RH - Basework for multithreading integration
06/02/2026 - RH - Fixes to the RTOS declarations 
06/14/2026 - RH - Added Startup sequence for IMU
06/14/2026 - RH - Battery Voltage display for serial log
06/16/2026 - RH - Testing for IMU; without battery testing which will need to be done independently 
*/

#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <MS5611.h>
#include "startup/imu_init.h"
#include <SimpleBatteryMonitor.h>

// Note that the GPIO number is offset by one so D2 is actually GPIO3 instead of GPIO2

// #define LED1_PIN 3 // 2 + 1
// #define LED2_PIN 5 // 4 + 1

//Sensors
// Adafruit_LSM6DSO32 dso32;
// MS5611 baro(0x77);

//Battery
SimpleBatteryMonitor battery;


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
    Continuity in batteries/ voltage
    IMU check
    Accelerometer Check
    Barometer Check

    For any vTaskDelete(NULL) need to add fail system or something for device to do instead of proceeding

  */
  Serial.println("Starting device. Commencing Start up sequence.");

  //Battery Checks
  // Serial.print("Reading Battery Voltage.");

  // Configure: ADC pin, R1, R2, Vref, ADC resolution
  // battery.configure(14, 330000, 100000, 3.3, 4096); //May have to change these numbers to fit the ESP
  //                 // GPIO5, 330kΩ, 100kΩ, 3.3V, 12-bit
  //                 // 5, 330000, 100000, 3.3, 4096
  //                 // pin14, ?, 1000000, 3.3, ?)
  // battery.begin();

  // float voltage = battery.readVoltage();
  // Serial.print("Battery Voltage is currently: ");
  // Serial.println(voltage);

  // if (voltage < 3.4) {
  //     Serial.println("CRITICAL: Battery too low for safe operation!");
  //     vTaskDelete(NULL);
  // }

  // if (voltage < 3.6) {
  //     Serial.println("WARNING: Battery low! Re-charge battery soon. Startup continuing...");
  // }

  // Serial.println("Battery voltage OK. Continuing bootprocess...");
  
  //IMU initilisation 
  //initIMU();

  Serial.println("IMU in check, all systems running. Continuing boot process...");
  vTaskDelete(NULL);
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  


}

//Begin placeholder loops

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

  Wire.begin(5,6);

  //IMU Testing
  Serial.println("Direct IMU init test...");
  initIMU();
  Serial.println("IMU init finished.");
 
  
  

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
    0);        

  xTaskCreatePinnedToCore(
    fileLogging,   
    "fileLogging",   
    10000,     
    NULL,      
    1,          
    &TaskFileLogging,   
    0);        

  xTaskCreatePinnedToCore(
    userTests,   
    "TestMode",   
    10000,     
    NULL,      
    1,          
    &TaskUserTests,   
    0);        

  xTaskCreatePinnedToCore(
    shutDown,   
    "PoweringOff",   
    10000,     
    NULL,      
    1,          
    &TaskShutdown,   
    0);        

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

