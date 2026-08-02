/**
07/08/2026 - RH - Created basics for file logging based on parameters on GitHub Ticket
*/

#pragma once
#include <Arduino.h>

void loggerInit();

void logSensors();

void loggerArmed();

void logEvents(const char* event, const char* device, const char* notes = "");

void logLaunch();

void endLogging();

bool loggingActive();

// Time in milliseconds to be logged once device is "ready"
unsigned long loggingTime();