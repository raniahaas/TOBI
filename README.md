# TOBI
Telemetry, Orientation and Barometric Instrument Flight Computer used for the University of Cincinnati's Rocketry Club's Projects.\
<img width="410" height="214" alt="TOBI_Logo" src="https://github.com/user-attachments/assets/6365c67b-6619-49ff-923c-a769e70f5e53" />

# Introduction
TOBI, from the Japanese word 飛び（とび「tobi」）meaning to soar, was born to serve as a custom avionics bay as a foundation for the University of Cincinnati's Rocketry Club's future space attempt, as well as a custom device personally. The overall goal for final development is to have a working custom flight computer that can log data, determine events, and fire pyrotechniques, all independently.

This repository is still in _**active development**_, but is intended as a clean starting ground to learn, teach, and develop further knowledge of avionics. 

| Goals | Method(s) of Execution |
| ------------- | ------------- |
| Multithreading | Using FreeRTOS, use the two cores on the ESP32-S3 to monitor and log events while another core stores data. This became a notable issue in past iterations where often data was missed, overlapped, or entirely logged improperly. |
| Event Logging | More specifically, events for apogee, main and drogue deployment, second-stage ignition, launch, and landing. |
| Pyrotechnics | Simulate the ignition of the second stage, wire cutters, and parachute deployment that current prefab electronics for model rocketry execute. This will become a full feature later on in the following year. |
| Wi-Fi Integration | Mostly, the ESP32 is known for its Wi-Fi capabilities. In our case, I will be utilising the Wi-Fi for data transfer and to check sensor continuity. This will be the main interface that the user uses to communicate with the device externally to the development. |
| Redundancy | Logging the sensors for data and comparing their redundancy with other computers or often COTS components to ensure our code is always performing to the best of its ability while also being the best option financially and methodologically.|

## Project Structure
```
tobiru/
├── data/
│   └── assets/          
│       └── TOBI_Logo.png
├── include/
│   └── flightState.h      # Hardware headers for the flight state
├── lib/                   # Project-specific libraries
├── src/
│   ├── startup/            
|       └── imu_init.cpp   # Initialise IMU datarates
|       └── imu_init.h
|       └── startupSeq.cpp # Start device and initialise accelerometer and barometer datarates; battery voltages
|       └── startupSeq.h
│   ├── globals.h          # Global elements
|   ├── globals.cpp
│   ├── main.cpp           # Task creations
│   └── wifiSetup.h        # Web dashboard with the Wi-Fi interface
├── test/                  # Unity test suites
│   ├── test_flightstate/
|       └── test_main.cpp
│   └── test_telemetry/
|       └── test_main.cpp
└── platformio.ini
```

## Device
| Insturment | Name |
| ---------- | ---- |
| Accelerometer | [Adafruit LSM6DSO32](https://www.digikey.com/en/products/detail/adafruit-industries-llc/4692/13250946) |
| Microcontroller | [XIAO SEEED ESP32-S3](https://www.digikey.com/en/products/detail/seeed-technology-co-ltd/113991114/19285530) |
| Barometer | [TE MS5611](https://www.digikey.com/en/products/detail/te-connectivity-measurement-specialties/MS561101BA03-50/5277445) |
| PCB | See KiCAD Files |
| Mosfet | [Toshiba T2N7002](https://www.digikey.com/en/products/detail/toshiba-semiconductor-and-storage/T2N7002BK-LM/5409245) |
| Additional elements | Battery terminal, 2-prong screw terminal, 4-prong screw terminal, resistors & transistors | 

## Project Status
- [X] Create multithreaded code for sensor logging (FreeRTOS)
- [X] Have a flight state machine for pre-launch, launch, and landing
- [X] Test functions before final implementation (Unity)
- [X] Local Wi-Fi server for live telemetry data and downloading post-flight data
- [X] PCB integration testing
- [ ] Use pyrotechniques for deploying main and drogue chutes
- [ ] Test flights!

## Building and Flashing Device
The native PlatformIO extension allows the user to flash to the device by simply validating the code and 'pushing' it onto the device. 
<img width="1504" height="870" alt="Flashing and validating code in Visual Studio Code with PlatformIO" src="https://github.com/user-attachments/assets/71fdf0ad-653e-4322-94ea-eec45a938b84" />
You can additionally flash the code manually by:
```
# Build and upload firmware to the flight computer
pio run -e esp32s3 -t upload

# Open serial monitor
pio device monitor
```

## Flight State
There are three main flight states: startup, ready, and landing. Startup occurs as soon as the device is powered on and will switch to ready once all checks are complete. In the ready state, the user on the front-end server can arm and disarm the device to start recording instruments for flight. Once events are detected and landing has been determined, the device will enter landing mode and produce summary information as well as downloads for CSV files for the user. 

| State | Description |
| ----- | ----------- |
| startup |	Sensor initialization runs (runStartupSequence()), Wi-Fi starts up |
|ready |	Initialisation complete, device idle, user can access webpage |
| armed	| Event detection is live — launch/apogee/landing checks run every cycle in shutDown()|
| landed	| Landing confirmed; event detection stops, flight summary data is finalised|

Transitions are currently one-directional (no state is re-entered), driven by main.cpp on startup/arm and by the event detection logic on landing confirmation.

## Flight Events
Key flight events are detected by a pure, hardware-independent function (updateFlightState), so the same logic that flies on the device can also be validated on a laptop with no board attached.

| Event | Trigger Condition |
| ----- | ----------------- |
|Launch |	Z-axis acceleration exceeds a threshold (~15 m/s²)|
|Motor Burnout | Gs drop and at least 0.5s have passed |
|Apogee |	Altitude drops more than 50m below the recorded peak|
|Landing | Altitude returns near launch altitude, acceleration stabilises near 1G, device is stable for 3+ seconds|

## Testing
Testing is natively done with the combination of PlatformIO and Unity, where instruments are stripped, and logic functions are tested. Flight-critical logic (launch detection, apogee confirmation, landing verification) is extracted into pure, hardware-independent functions.

```
pio test -e native -f test_flightstate
```
platformio.ini defines two environments inside of testing:
- esp32s3: the physical hardware board the main code runs off of
- native: emulates real hardware to test functions purely


## Safety
This project is intended for use in HPR (high-powered rocketry) and uses pyrotechniques to deploy parachutes and allow for second-stage ignitions. All safety protocols must follow NAR safety codes and are built in accordance with a recoverable HPR two-stage flight. Do not fire, wire, or test pyrotechnic channels without knowledge of the code, device, and user-controlled testing deployment. 

