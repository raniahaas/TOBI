/**
07/14/2026 - RH - Native platformio unit testing, stripped globals.h and main.cpp
*/

#pragma once
#include <cstdlib>

struct FlightState {
    bool launched = false;
    bool apogeeReached = false;
    bool landed = false;
    float launchAltitude = 0;
    float maxAltitudeSeen = 0;
    float apogee = 0;
    unsigned long landedConfirmedStart = 0;
};

//Sim data for calculations
struct FlightInputs {
    float accelZ;
    float altitude;
    unsigned long nowMs;
};

//Dumbed down logic for updating flight status
inline FlightState updateFlightState(FlightState s, const FlightInputs& in) { //By value here not reference
    if (!s.launched && in.accelZ > 15.0f) {
        s.launched = true;
        s.launchAltitude = in.altitude;
    }

    if (s.launched && in.altitude > s.maxAltitudeSeen) {
        s.maxAltitudeSeen = in.altitude;
    }

    if (s.launched && !s.apogeeReached && (s.maxAltitudeSeen - in.altitude) > 50.0f) {
        s.apogeeReached = true;
        s.apogee = s.maxAltitudeSeen;
    }

    if (s.apogeeReached && !s.landed) {
        bool nearGround = (in.altitude < s.launchAltitude + 20.0f);
        bool stable = (fabs(in.accelZ - 9.81f) < 1.0f);

        if (nearGround && stable) {
            if (s.landedConfirmedStart == 0){
                s.landedConfirmedStart = in.nowMs;
            } else if (in.nowMs - s.landedConfirmedStart > 3000) {
                s.landed = true;
            }
        } else {
            s.landedConfirmedStart = 0;
        }
    }

    return s;
}