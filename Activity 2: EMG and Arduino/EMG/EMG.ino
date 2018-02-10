#include "CurieIMU.h"
#include "CurieTimerOne.h"
void setup() {
 Serial.begin(115200);   //Initialize Serial Communication
 while(!Serial);     //Wait for connection
 CurieIMU.begin();
 CurieTimerOne.start(5000,EMG); //Create an interrupt function for taking EMG sample
 }
 
void loop() {
}
void EMG(){   //Interrupt for taking EMG sample
   Serial.print(micros());  //print out time in seconds
   Serial.print("\t");
   Serial.println(analogRead(A0));  //Print out the raw EMG values into serial monitor
}

