/**
06/30/2026 - RH - Created shared global elements
07/06/2026 - RH - Added gyro, flight summary, csvReady; unified flightStatus naming
*/
#pragma once
#include <Arduino.h>

//Sensors
#include <Adafruit_LSM6DSO32.h>
#include <SimpleBatteryMonitor.h>
extern Adafruit_LSM6DSO32 dso32;
extern SimpleBatteryMonitor battery;

//Flight status
extern String flightStatus;
extern bool   csvReady;

//IMU data
extern float g_accelX;
extern float g_accelY;
extern float g_accelZ;

extern float g_gyroX;
extern float g_gyroY;
extern float g_gyroZ;

//Barometer
extern float g_barometer;

//Flight summary variables
extern float g_apogee;
extern float g_drogue;
extern float g_main;
extern float g_maxVel;
extern float g_avgVel;
extern float g_timeToApogee;
extern float g_timeToMain;