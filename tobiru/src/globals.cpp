/**
06/30/2026 - RH - Created shared global elements
07/06/2026 - RH - Moved sensors into global and added more post flight variables
*/
#include "globals.h"

//Sensors
Adafruit_LSM6DSO32 dso32;
SimpleBatteryMonitor battery;

//Status state
String flightStatus = "startup";
bool   csvReady    = false;

//IMU data
float g_accelX = 0.0f;
float g_accelY = 0.0f;
float g_accelZ = 0.0f;

float g_gyroX  = 0.0f;
float g_gyroY  = 0.0f;
float g_gyroZ  = 0.0f;

//Barometer data
float g_barometer = 0.0f;

//Flight summary variables
float g_apogee      = 0.0f;
float g_drogue      = 0.0f;
float g_main        = 0.0f;
float g_maxVel      = 0.0f;
float g_avgVel      = 0.0f;
float g_timeToApogee = 0.0f;
float g_timeToMain  = 0.0f;