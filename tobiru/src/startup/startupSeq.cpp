#include "startupSeq.h"
#include "imu_init.h"
#include <SimpleBatteryMonitor.h>
#include <Arduino.h>





void runStartupSequence() {
  /*Things to check on startup:
    Continuity in batteries/ voltage
    IMU check
    Accelerometer Check
    Barometer Check

    For any vTaskDelete(NULL) need to add fail system or something for device to do instead of proceeding

  */

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
  
  Serial.println("Starting device. Commencing Start up sequence.");
  initIMU();
  Serial.println("IMU in check, all systems running. Continuing boot process...");
  

}

