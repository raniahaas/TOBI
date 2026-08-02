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
#include "fileLogging.h"


//Tasks
TaskHandle_t TaskFileLogging;
TaskHandle_t TaskEventLogging;
TaskHandle_t TaskUserTests;
TaskHandle_t TaskShutdown;
TaskHandle_t TaskIMU;
TaskHandle_t TaskBaro;
TaskHandle_t TaskBattery;

const int led1 = 3;
const int led2 = 5;

//Pyro pins for main and drouge; dont forget about offset when testing
const int PYRO_DROUGE = 5;
const int PYRO_MAIN = 9;

//Landed state variables
static float         maxAltitudeSeen    = 0;
static float         launchAltitude     = 0;
static bool          launched           = false;
static bool          apogeeReached      = false;
static unsigned long landedConfirmStart = 0;
static bool burnoutLogged = false;
static bool drogueDeployed = false;
static bool mainDeployed = false;
static unsigned long landingConfirmed = 0;
static unsigned long burnoutTime = 0;

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

    logSensors();

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
    if (flightStatus == "armed") {

    }
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


      //Detect launch when Z accel spike <1.5G
      if (!launched && az > 15.0f) {
        launched       = true;
        launchAltitude = alt;
        logLaunch();
        Serial.println("LAUNCH DETECTED");
      }

      if (!launched) {
        vTaskDelay(pdMS_TO_TICKS(50));
        continue;
      }

      //Calculate motor burnout based on Gs dropping and at least 0.5s passing
      if (!burnoutLogged && loggingTime() > 500) {
        if(az < 12.0f) {
            burnoutTime = loggingTime();
            burnoutLogged = true;
            logEvents("MOTOR BURNOUT", "Accelerometer", "Acceleration dropping post launch, motor burnout occuring.");
            Serial.println("Motor burnout detected!");
        }
      }

      //Track altitude for apogee
      if (alt > maxAltitudeSeen) {
        maxAltitudeSeen = alt;
      }

      //Detect apogee when device droppes 50m below the peak altitude
      if (launched && !apogeeReached && (maxAltitudeSeen - alt) > 50.0f) {
        apogeeReached = true;
        g_apogee = maxAltitudeSeen;
        logEvents("APOGEE", "Barometer and Accelerometer", "Altitude dropping at least 50 meters below max recorded height.");
        Serial.print("APOGEE DETECTED: ");
        Serial.println(g_apogee);
      }

      //Detect landing when near ground state and stable for 3+s 
      if (apogeeReached && !drogueDeployed) {
        digitalWrite(PYRO_DROUGE, HIGH);
        delay(500);
        digitalWrite(PYRO_DROUGE, LOW);
        drogueDeployed = true;
        g_drogue = alt;
        logEvents("DROGUE_DEPLOYED", "Pyro & Barometer & Accelerometer", "Drogue deployment after e-match.");
        Serial.println("Drogue Deployed!");
      }   

      //Main deployment
      const float MAIN_DEPLOY_ALT = launchAltitude + 150.0f;
      if(drogueDeployed && ! mainDeployed && alt < MAIN_DEPLOY_ALT) {
        digitalWrite(PYRO_MAIN, HIGH);
        delay(500);
        digitalWrite(PYRO_MAIN, LOW);
        mainDeployed = true;
        g_main = alt;
        logEvents("MAIN_DEPLOY", "Pyro & barometer", "Main pyros have been fired");
        Serial.println("MAIN DEPLOYED");
      }
    
      //Landing detection
      if(mainDeployed) {
        bool nearGround = (alt < launchAltitude + 20.0f);
        bool stable     = (abs(az - 9.81f) < 1.0f);

        if (nearGround && stable) {
          if (landedConfirmStart == 0) {
            landedConfirmStart = millis();
          } else if (millis() - landedConfirmStart > 3000) {

            g_timeToApogee = apogeeReached ? (float)loggingTime() : 0;
            g_timeToMain = mainDeployed ? (float)loggingTime() : 0;

            flightStatus = "landed";
            Serial.println("LANDED");
          }
        } else {
          landedConfirmStart = 0;
        }
      }
    }

    //20 Hz for eventes
    vTaskDelay(pdMS_TO_TICKS(50)); 
  }
}


void userTests(void *pvParameters) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  Wire.begin(5, 6);

  //PYRO pins
  pinMode(PYRO_DROUGE, OUTPUT); digitalWrite(PYRO_DROUGE, LOW);
  pinMode(PYRO_MAIN, OUTPUT); digitalWrite(PYRO_MAIN, LOW);

  //Intialise state
  flightStatus = "startup";

  runStartupSequence();
  startWifi();
  loggerInit();

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
  xTaskCreatePinnedToCore(writeEvents,   "eventLogging", 8192, NULL, 2, &TaskEventLogging, 0);
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