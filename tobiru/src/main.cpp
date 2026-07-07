/**
05/22/2026 - RH - Basework for multithreading integration
06/02/2026 - RH - Fixes to the RTOS declarations
06/14/2026 - RH - Added Startup sequence for IMU
06/14/2026 - RH - Battery Voltage display for serial log
06/16/2026 - RH - Testing for IMU; without battery testing independently
                - Moved Startup function to separate file
06/30/2026 - RH - Wired flightStatus transitions for WiFi dashboard
07/06/2026 - RH - Edits for incoorperating global variables
*/

#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <MS5611.h>
#include "startup/startupSeq.h"
#include <SimpleBatteryMonitor.h>
#include "globals.h"
#include "wifiSetup.h"


//Tasks
TaskHandle_t TaskFileLogging;
TaskHandle_t TaskEventLogging;
TaskHandle_t TaskUserTests;
TaskHandle_t TaskShutdown;
TaskHandle_t TaskIMU;
TaskHandle_t TaskBaro;

const int led1 = 3;
const int led2 = 5;

//Landed state variables
static float         _maxAltitudeSeen    = 0;
static float         _launchAltitude     = 0;
static bool          _launched           = false;
static bool          _apogeeReached      = false;
static unsigned long _landedConfirmStart = 0;

//IMU writing
void imuWrite(void *pvParameters) {
  for (;;) {
    sensors_event_t accel, gyro, temp;
    dso32.getEvent(&accel, &gyro, &temp);

    //Write into globals to modify and grab
    g_accelX = accel.acceleration.x;
    g_accelY = accel.acceleration.y;
    g_accelZ = accel.acceleration.z;

    g_gyroX  = gyro.gyro.x;
    g_gyroY  = gyro.gyro.y;
    g_gyroZ  = gyro.gyro.z;

    vTaskDelay(pdMS_TO_TICKS(100)); //10Hz
  }
}

//Barometer
void readBarometer(void *pvParameters) {
  for (;;) {
    //Once surface mounts are applied test
    // g_barometer = baro.getPressure();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

//Event logging
void writeEvents(void *pvParameters) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

//File logging
void fileLogging(void *pvParameters) {
  for (;;) {
    // log into csv file
    //csvReady = true; for download
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

//Landing detection and shutdown
//Landing detection will be separate but required for shutdown as well
void shutDown(void *pvParameters) {
  for (;;) {

    if (flightStatus == "armed") {

      float az  = g_accelZ;
      float alt = g_barometer; //replace with real altitude from baro

      //Detect launch when Z accel spike above ~1.5G
      if (!_launched && az > 15.0f) {
        _launched       = true;
        _launchAltitude = alt;
        Serial.println("LAUNCH DETECTED");
      }

      //Track altitude for apogee
      if (_launched && alt > _maxAltitudeSeen) {
        _maxAltitudeSeen = alt;
      }

      //Detect apogee when device droppes 50m below the peak altitude
      if (_launched && !_apogeeReached && (_maxAltitudeSeen - alt) > 50.0f) {
        _apogeeReached = true;
        g_apogee       = _maxAltitudeSeen;
        Serial.print("APOGEE DETECTED: ");
        Serial.println(g_apogee);
      }

      //Detect landing when near ground state and stable for 3+s 
      if (_apogeeReached) {
        bool nearGround = (alt < _launchAltitude + 20.0f);
        bool stable     = (abs(az - 9.81f) < 1.0f);

        if (nearGround && stable) {
          if (_landedConfirmStart == 0) {
            _landedConfirmStart = millis();
          } else if (millis() - _landedConfirmStart > 3000) {
            //need to add data here stil
            g_drogue       = 0;
            g_main         = 0;
            g_maxVel       = 0;
            g_avgVel       = 0;
            g_timeToApogee = 0;
            g_timeToMain   = 0;

            flightStatus = "landed";
            Serial.println("LANDED");
          }
        } else {
          _landedConfirmStart = 0;
        }
      }
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

//Test the device before flight
void userTests(void *pvParameters) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  Wire.begin(5, 6);

  //Intialise state
  flightStatus = "startup";

  runStartupSequence();
  startWifi();

  flightStatus = "ready";

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
  xTaskCreatePinnedToCore(writeEvents,   "eventLogging", 4096, NULL, 2, &TaskEventLogging, 0);
  xTaskCreatePinnedToCore(fileLogging,   "fileLogging",  4096, NULL, 1, &TaskFileLogging,  0);
  xTaskCreatePinnedToCore(userTests,     "TestMode",     4096, NULL, 1, &TaskUserTests,    0);
  xTaskCreatePinnedToCore(shutDown,      "PoweringOff",  4096, NULL, 1, &TaskShutdown,     0);
 
  //Core 1 Tasks
  xTaskCreatePinnedToCore(imuWrite,      "imu",          4096, NULL, 2, &TaskIMU,          1);
  xTaskCreatePinnedToCore(readBarometer, "barometer",    4096, NULL, 1, &TaskBaro,         1);   

}

void loop() {
  WiFiInterface(); 
}