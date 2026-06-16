/**
06/02/2026 - RH - 
*/

#pragma once
#include <Adafruit_LSM6DSO32.h>
//doing pragma once here so it truly only does run once
//could cause issues later down the line if constantly checking

extern Adafruit_LSM6DSO32 dso32;

void initIMU();