/**
06/02/2026 - RH - 
*/

//Source: https://cdn-learn.adafruit.com/downloads/pdf/lsm6dsox-and-ism330dhc-6-dof-imu.pdf 

#include "imu_init.h"
#include <Arduino.h>

Adafruit_LSM5DSO32 dso32;

//Ensure serial.begin is already called before this function
bool initIMU() {
    //Checking for continuity, setting ranges, etc
    //Add these checks later to WiFi Server so user can see without Serial Monitor

    Serial.println("Initlizing IMU...");
    if (!dso32.begin_I2C()){
        //Add possible checks here for CS, SCK, MISO, or MOSI ports
        Serial.println("Failed to find LSM6DSO32.");
        
        //Retry
        while(1){
            delay(10);
        }
    }

    //Found device
    Serial.println("LSM6DSO32 Found!");


    Serial.println("Starting Accelerometer checks...");
    //Set ranges for accelerometer
    dso32.setAccelRange(LSM6DSO32_ACCEL_RANGE_8_G);
    //Most likely will always be set to 32Gs for FC but can be modified for testing
    Serial.print("Accelerometer range set currrently to: ");
    switch(dso32.getAccelRange()) {
        case LSM6DSO32_ACCEL_RANGE_4_G:
            Serial.println("+-4G");
            break;
        case LSM6DSO32_ACCEL_RANGE_8_G:
            Serial.println("+-8G");
            break;
        case LSM6DSO32_ACCEL_RANGE_16_G:
            Serial.println("+-16G");
            break;
        case LSM6DSO32_ACCEL_RANGE_32_G:
            Serial.println("+-32G");
            break;
    }

    Serial.print("Accelerometer data rate is currrently set to: ");
    switch(dso32.getAccelDataRate()) {
        case LSM6DS_RATE_SHUTDOWN:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_12_5_HZ:
            Serial.println("12.5 Hz");
            break;
        case LSM6DS_RATE_26_HZ:
            Serial.println("26 Hz");
            break;
        case LSM6DS_RATE_52_HZ:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_104_HZ:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_208_HZ:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_416_HZ:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_833_HZ:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_1_66K_HZ:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_3_33K_HZ:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_6_66K_HZ:
            Serial.println("0 Hz");
            break;
    }



    Serial.println("Starting Gyroscope checks...");
    //Set ranges for gyro
    Serial.print("Gyro range currently set to: ");
    switch (dso32.getGyroRange()){
        case LSM6DS_GYRO_RANGE_125_DPS:
            Serial.println("125 degrees per second.");
            break;
        case LSM6DS_GYRO_RANGE_250_DPS:
            Serial.println("250 degrees per second.");
            break;
        case LSM6DS_GYRO_RANGE_500_DPS:
            Serial.println("500 degrees per second.");
            break;
        case LSM6DS_GYRO_RANGE_1000_DPS:
            Serial.println("1,000 degrees per second.");
            break;
        case LSM6DS_GYRO_RANGE_2000_DPS:
            Serial.println("2,000 degrees per second.");
            break;
        case ISM330DHCX_GYRO_RANGE_4000+DPS:
            Serial.println("Unsupported range for the DSO32--Please modify your selection.");
            break;
    }

    

}
