# Beginnings: Multithreading and Sensors
Introductory code and LED development was done by following this article [here](https://randomnerdtutorials.com/esp32-freertos-arduino-tasks/).

Note the ESP32 being used in the article is slightly different from the one I am using. I am currently running off of the ESP32-S3 by Seeed. 
![image.png](attachment:6d2e6f51-7616-4254-a6b7-1b70d1c84374:image.png)

This is the intial wiring I used following the article. The overarching idea is to specifically assign a core (1 in this case) to blink the LED every second. 
<img width="3024" height="4032" alt="Working with one LED on a single core" src="https://github.com/user-attachments/assets/57b8b31b-7069-4b54-8803-c2e99e4be2d3" />
The largest trial and tribulation I noted here (other than simply learning how to work with a very basic hardware system), is the GPIO number for the Seeed is different than the regular Arduino GPIO number. The indexing starts at 1, not 0, which did cause some hairs to be pulled. I then moved to actually working with two cores by using two separate LEDs, one blinking every second on core 1 and another blinking every half second on core 0. 
<img width="1596" height="618" alt="Serial Monitor logging results of multithreading two LEDs." src="https://github.com/user-attachments/assets/4b9de912-1e2c-4bfe-a112-a40ffc5b9584" />
Additionally, viewed similarly to the stack and heap, not exactly necessary currently, but for more advanced development further down the line, this will become handy.
<img width="1768" height="714" alt="Serial Monitor outputs of the stack and heap available" src="https://github.com/user-attachments/assets/c28e96cc-e168-488e-b1da-b318a053aff1" />

# PCB Practice and Development in KiCAD
This section initially follows the tutorials from [HTM Workshop](https://www.youtube.com/playlist?list=PLUOaI24LpvQPls1Ru_qECJrENwzD7XImd) on schematics, custom parts, and PCB design in KiCAD. Initally, this was not a critical piece for the development of TOBI, but as an overall for my advancement of knowledge of integration with hardware and understanding the day to day activites of an embedded systems engineer, under my mentorship this deemed to be a critical step in my knowledge.
### Practicing Schematics
The goal here was to re-create a schematic of a SPO2 Module into KiCAD. This just helped me get a better lay of the land when it came to working with KiCAD, running the electical rule checker, ensuring there are no connect flags properly placed, and the general wiring. 
<img width="1505" height="940" alt="Final SPO2 Module" src="https://github.com/user-attachments/assets/c1d5aac8-71c9-48db-8771-28a1235185e3" />
Additionally, I also learned how to make custom symbols to be added into schematics. Definitley one of the more useful tidbits to know when working with some lesser known avionics parts. 
<img width="1503" height="896" alt="Custom MAZ30102 Module for KiCAD" src="https://github.com/user-attachments/assets/da84342c-d646-467f-8d63-9f66f037a9d9" />
