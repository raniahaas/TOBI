# TOBI
Telemetry, Orientation and Barometric Instrument Flight Computer used for the University of Cincinnati's Rocketry Club's Projects.\
<img width="410" height="214" alt="TOBI_Logo" src="https://github.com/user-attachments/assets/6365c67b-6619-49ff-923c-a769e70f5e53" />

# Introduction
TOBI, from the Japanese word 飛び（とび「tobi」）meaning to soar, was born to serve as a custom avionics bay as a foundation for the University of Cincinnati's Rocketry Club's future space attempt, as well as a custom device personally. The overall goal for final development is to have a working custom flight computer that can log data, determine events, and fire pyrotechniques, all independently.

| Goals | Method(s) of Execution |
| ------------- | ------------- |
| Multithreading | Using FreeRTOS, use the two cores on the ESP32-S3 to monitor and log events while another core stores data. This became a notable issue in past iterations where often data was missed, overlapped, or entirely logged improperly. |
| Event Logging | More specifically, events for apogee, main and drogue deployment, second-stage ignition, launch, and landing. |
| Pyrotechnics | Simulate the ignition of the second stage, wire cutters, and parachute deployment that current prefab electronics for model rocketry execute. This will become a full feature later on in the following year. |
| Wi-Fi Integration | Mostly, the ESP32 is known for its Wi-Fi capabilities. In our case, I will be utilising the Wi-Fi for data transfer and to check sensor continuity. This will be the main interface that the user uses to communicate with the device externally to the development. |
| Redundancy | Logging the sensors for data and comparing their redundancy with other computers or often COTS components to ensure our code is always performing to the best of its ability while also being the best option financially and methodologically.|

This repository is still in _**active development**_, but is intended as a clean starting ground to learn, teach, and develop further knowledge of avionics. 

## Device
| Insturment | Name |
| ---------- | ---- |
| Accelerometer | [Adafruit LSM6DSO32](https://www.digikey.com/en/products/detail/adafruit-industries-llc/4692/13250946) |
| Microcontroller | [XIAO SEEED ESP32-S3](https://www.digikey.com/en/products/detail/seeed-technology-co-ltd/113991114/19285530) |
| Barometer | [TE MS5611](https://www.digikey.com/en/products/detail/te-connectivity-measurement-specialties/MS561101BA03-50/5277445) |
| PCB | See KiCAD Files |
| Mosfet | [Toshiba T2N7002](https://www.digikey.com/en/products/detail/toshiba-semiconductor-and-storage/T2N7002BK-LM/5409245) |
| Additional elements | Battery terminal, 2-prong screw terminal, 4-prong screw terminal, resistors & transistors | 



