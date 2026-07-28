Further development and contributions to TOBI and further UCRC projects will often follow the same structure for code development. We encourage you to play around with your own custom code on **both** the hardware and software side to see how it plays out before committing and testing to an open issue.
# Following a new Issue
<img width="1505" height="730" alt="Issues Tab for the TOBI repository" src="https://github.com/user-attachments/assets/f5e9d932-949f-452d-a2bd-0423ed30d690" />
Under the issues tab, there will often be several tickets for items that need work on. Clicking on each ticket will give you a more in-depth description of the issue at hand, possible methods of execution, considerations for implementation, and members' feedback and communication on issues in development. Please comment and signify if you are picking up a ticket so it can properly be assigned to you, letting other members know who is working on what code. Often, each ticket will come with a branch attached to it; however, if there is no branch, you can 'Create a branch' for development designated directly to a ticket. Find a new issue within the ticket that is creating a larger scope than intended? Create a sub-ticket and signify the additional focus and add a new branch

# Documenting code
There are two main locations for documenting your code: headers and modified locations. At the top of all files are headers that look like such and include the date, user, and what modifications were made.
``` 
/** 07-14-2026 - RH - Modified main.cpp to include data logging for barometer and accelerometer
*/
```
This is great for helping keep track of who did what work and also gives you credit to everyone else when you make a great function! Additionally, when modifying large blocks of code, often in main.cpp or other commonly shared files, indicate your **START** & **STOP** locations in case we need to roll back code; we can easily identify where issues are possibly occurring from. In files that are either custom to your ticket or are short in nature, simple one or two-line comments about the lines added in addition to the headers are fine. Additional comments on what functions do are not required in START/STOP scenarios if they are not needed.
```
void setup() {
  Serial.begin(115200);
  delay(3000);

  // RH - BEGIN
  //Start reading from SDA and SCL ports
  Wire.begin(5, 6);
  //RH - END
}
```

# Pushing code
<img width="964" height="666" alt="GitHub Desktop app" src="https://github.com/user-attachments/assets/01e24ff2-76f4-47f5-a11f-ccdb37756821" />
For good practice, it is always a good idea when you are working on a project to push your code after you accomplish a part, whether big or small. Add your commit message, which is a general overview of what you worked on as well as a description where you can indicate what you worked on, what still needs to be worked on, and what still may need additional research or testing. Make sure to push to origin once you push locally.

# globals.h & globals.cpp
There are two main types of [variables](https://www.geeksforgeeks.org/cpp/scope-of-variables-in-c/) in programming: local and global. Local is either local to a function, which means they cannot be accessed externally to that function, whereas global variables can be accessed anywhere in the code. When we are working in an embedded system and across several files, our scope of local and globals shifts slightly. We have local function variables, local file variables, and globals. As noted, local function variables can only be accessed within said function. If we have two variables of the same name, they must both be local variables; otherwise, you are working with one global variable. 
```
int i; //i is a local variable to the file or a global to just this file

if (int j == 0)
  i++;

if (j == 1); //j is a separate variable here from the loop above
  i++;
  j++; // this is a new local variable also named j
```
When you call a function from another file, you can often access it directly based on your imports.
```
#include 'startup.h'

if (startup == true) {
  //We can call any function from startup.cpp directly in our file since we #include 'd' the file
  startWifi(); //a function from startup.cpp
}
```
However, when we are accessing a variable across multiple files and constantly needing and updating these values, we create a global variable file. This is where our globals.cpp and globals.h start to play. For many of our current statuses and accessing any of the hardware elements, we use globals.cpp.
```
//globals.h
#pragma once
#include <Arduino.h>

//Sensors
#include <Adafruit_LSM6DSO32.h>
#include <SimpleBatteryMonitor.h>
extern Adafruit_LSM6DSO32 dso32;
extern SimpleBatteryMonitor battery;


//globals.cpp

#include "globals.h"

//Sensors
Adafruit_LSM6DSO32 dso32;
SimpleBatteryMonitor battery;
```
globals.h, like any other [h file](https://www.geeksforgeeks.org/cpp/header-files-in-c-c-with-examples/), is a header file where we keep all the declarations of globals for the file and functions so they can be used and called from different external files. We will 'extern'ally call these inside of our header file, and later instantiate them inside of our C++ file. When you are adding a new variable as a global, use extern and the name of your variable as well as actually declaring it inside of globals.cpp. For example: let's simulate adding a variable to save our current flight status to. 
```
extern flightStatus //in globals.h

bool flightStatus = false; // in globals.cpp
```

# Locally testing code with hardware
When you believe your functions are complete and you need to start testing with hardware/the avionics bay, there are a few things to consider. With local testing, we can use the Serial Monitor to send debug messages to our computer to debug and ensure code is running as planned using serial print statements.
```
Serial.print("Avionics bay starting...");
```
Ensure when you are using the Serial Monitor, the correct port is being used. It will often be a USBC port from the XIAO SEEED ESP32S3 or Espressif; when connecting for the first time, you may have to allow Bluetooth permissions for the device. Make sure you press START monitoring to see these local messages. Closing the port is critical when you are constantly flashing to the device, as it will cause failures with a return message of 'busy port.' It is also a good idea to add delay statements as once the code is flashed, the device will commence the startup sequence.
<img width="1499" height="943" alt="Using the serial monitor to debug locally in VSC." src="https://github.com/user-attachments/assets/21def8f5-ae64-4dc3-a21a-6a83ffb2d256" />
To write a delay statement, simply, before running your function, add the following:
```
delay (500); //Where our time here is in ms
```


# Unit testing & Truth tables
With Platformio, it natively integrates [Unity](https://github.com/ThrowTheSwitch/Unity), a form of [testing](https://www.geeksforgeeks.org/software-testing/unit-testing-software-testing/) that allows you to focus on specific functions and ensure they are delivering correct results in tested scenarios. There are three main parts to unit testing: truth tables, stripping your code, and integrating truth table conditions for testing.

To start, we must look at any possible edge cases as well as determine the functionality under presumed conditions. For the purpose of this example, we will be following a function that determines whether the device has reached apogee or not. 
<img width="1258" height="656" alt="Truth tables for functionality of the flight state and various functions." src="https://github.com/user-attachments/assets/15478d99-dd6b-4b2e-be3c-a88fb0ecfdfd" />
There are two key variables to determining apogee: acceleration and altitude. Note: we are only looking at these two variables based on how the apogee logic in main was previously written.
```
if (flightStatus == "armed") {
  FlightInputs in{ g_accelZ, g_barometer, millis() };
...
  if (state.launched && !wasLaunched) Serial.println("LAUNCH DETECTED"); //State is being determined externally in flightState.h in our 'includes' folder
  if (state.apogeeReached && !wasApogee) { //determined if the next point recorded is ≤50m than the next point
    g_apogee = state.apogee;
    Serial.print("APOGEE DETECTED: ");
    Serial.println(g_apogee);
  }
  ...
}
```
With our two inputs of acceleration and barometer data (altitude), we translate this over to our truth tables. The key to testing data and finding edge cases is to determine how a function behaves under correct inputs, incorrect inputs, and special scenarios. It is **_critical_** to list out all the possible issues and create test cases. For apogee, we can look at the rocket increasing, possible lag between two stages of ignition, and finally descent. (Based on the number of variables, is the minimum number of cases we should be testing) Then, we translate this code over into a simplified version to test our main code from.
<img width="1501" height="941" alt="VSC Screenshot of location for unit testing in Unity." src="https://github.com/user-attachments/assets/71cfd4ce-2a3a-44eb-a013-e89f6ce455fb" />
The native unit testing comes from a subfolder called test. Inside this folder, you can find other possible test subfolders; please ensure you create your own subfolder based on the methods you are currently testing. All of the code for apogee detection is occurring inside of main, so here we have test_main.cpp. Our test folder is where we are putting all of our example values from our truth tables into our functions.
```
void test_apogee_detected_after_50m_drop() {
  FlightState s;

  s = updateFlightState(s, {16.0f, 100.0f, 0}); //Test 1, returns FALSE
  s = updateFlightState(s, {17.0f, 90.0f, 50}); //Test 2, returns FALSE
  //Trigger Apogee
  s = updateFlightState(s, {9.81f, 500.0f, 100}); //Test 3, returns FALSE

  s = updateFlightState(s, {9.81f, 440.0f, 200}); //Test 4, returns TRUE

  TEST_ASSERT_TRUE(s.apogeeReached);
  TEST_ASSERT_EQUAL_FLOAT(500.0f, s.apogee);
}
```
Our actual logic for updating flight state and determining whether or not apogee has been reached is inside of our include folder, where we can create a C file with our modified logic from main.cpp. Why can't we use main and our original logic? This is because when we write our scripts to test functions such as apogee, we are constantly reading live barometric and accelerometer data, which we cannot use in general for our unit testing since the sensors would read incorrectly in our tests, but also since we use the variables for the sensors themselves. 
<img width="1508" height="946" alt="Include folder for Platformio Unity testing logic." src="https://github.com/user-attachments/assets/0c674f27-a958-42bc-aa8f-5198a40a256f" />
```
struct FlightState {
  bool launched = false;
  bool apogeeReached = false;
  bool landed = false;
  float launchAltitude = 0;
  float maxAltitudeSeen = 0;
  float apogee = 0;
  unsigned long landedConfirmedStart = 0;
}
...

inline FLightState updateFlightStatus(FlightState s, const FlightInputs& in) {
  ...

  if (s.launched && in.altitude > s.maxAltitudeSeen) {
    s.maxAltitudeSeen = in.altitude;
  }

  if (s.launched && !s.apogeeReached && (s.maxAltitudeSeen - in.altitude) > 50.0f) {
    s.apogeeReached = true;
    s.apogee = s.maxAltitudeSeen;
  }

  ...
}
```
As we can see, this is purely the logic, stripped from all reading of instruments. When we update our flight state, it is running through our flightState.h to determine whether or not apogee has been reached based on our logical conditions in the code. Finally, to test our code, we can run local commands to ensure all cases are being met. Running the following command in your terminal will allow you to test all of your cases.
```
Native: tobiru native$ pio test -e native -f test_flightstate
```
With a successful result, you know your code is all up to speed and working properly with your truth tables! 
<img width="1506" height="941" alt="Screenshot 2026-07-23 at 14 18 51" src="https://github.com/user-attachments/assets/7e62e7f8-87a1-47c3-95d2-4c3a12fc3ec4" />


# Peer reviewing code
Once you have finished your testing, before finally closing a ticket, have another member attempt to test your code (if plausible) and walk them through your code. Readability is key as this repo continues to be passed through several years and avionics teams; down the line, all code should be easily understood. Additionally, ensure that when demonstrating your code, there are no additional edge cases that you may have missed. Edge cases are often where large critical errors can occur, so if you aren't 100% with your finished code, please continue to ask other members and leads to review and ensure all edge cases have been demonstrated in unit testing. 

# Rebasing code
Rebasing code essentially allows you, once a ticket is finished, to close the ticket and merge the code with our main branch (remember, main is always the most updated code; this is what we are running launch day on!). Ensure that you have all of your code peer reviewed and checked with leads before finally pushing and rebasing. Once rebased, you're free to see all of your hard work in main and close your ticket!
```
git checkout main
git pull
git checkout branch-name
git rebase main
```
