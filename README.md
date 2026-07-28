# TOBI
Telemetry, Orientation and Barometric Instrument Flight Computer used for the University of Cincinnati's Rocketry Club's Projects.

# Introducction
TOBI, from the Japanese word 飛び（とび「tobi」）meaning to soar, was born to serve as a custom avionics bay as a foundation for the University of Cincinnati's Rocketry Club's future space attempt, as well as a custom device personally. The overall goal for final development is to have a working custom flight computer that can log data, determine events, and fire pyrotechniques, all independently.

| Goals | Method(s) of Execution |
| ------------- | ------------- |
| Multithreading | Using FreeRTOS, use the two cores on the ESP32-S3 to monitor and log events while another core stores data. This became a notable issue in past iterations where often data was missed, overlapped, or entirely logged improperly |
| Event Logging | More specifically events for apogee, main and drogue deployment, second stage ignition, launch, and landing. |
| Pyrotechnics | Simulate the ignition of the second stage, wire cutters, and parachute deployment that current prefab electronics for model rocketry execute. This will become a full feature later on in the following year. |
| Wi-Fi Integration | Mostly, the ESP32 is known for its Wi-Fi capabilities. In our case, I will be utilising the Wi-Fi as a method of data transfer and checking continuity of sensors. This will be the main interface that the user uses to communicate with the device externally to the development. |
| Redundancy | Logging the sensors for data and comparing their redunancy with other computers or often COTS components to ensure our code is always performing to the best of its ability while also being the best option financially and methilogically.|

This repository is still in _**active development**_, but is intended as a clean starting ground to learn, teach, and develop further knowledge of avionics. 


