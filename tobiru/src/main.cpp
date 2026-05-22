#include <Arduino.h>
// Note that the GPIO number is offset by one so D2 is actually GPIO3 instead of GPIO2

#define LED1_PIN 3 // 2 + 1
#define LED2_PIN 5 // 4 + 1

TaskHandle_t Task1;
TaskHandle_t Task2;

const int led1 = 3;
const int led2 = 5; //possibly may need to change

//Code section for tasks
void Task1code(void *pvParameters)  {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;) { //blink for 1 second
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    delay(1000);
  }
}

void Task2code(void *pvParameters){
  Serial.print("Task2 runnin on core ");
  Serial.println(xPortGetCoreID());

  for(;;) { //blink every 0.5 s
    digitalWrite(led2, HIGH);
    delay(700);
    digitalWrite(led2, LOW );
    delay(700);
  }
}

void setup() {
  Serial.begin(115200); //default baud rate
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  //task 1 to core 0
  xTaskCreatePinnedToCore(
    Task1code, 
    "Task1",   
    10000,      
    NULL,      
    1,         
    &Task1,     
    0);                   
  delay(500); 

  //task 2 to core 1
  xTaskCreatePinnedToCore(
    Task2code,   
    "Task2",   
    10000,     
    NULL,      
    1,          
    &Task2,   
    1);        
    delay(500); 
}


void loop() {

}

