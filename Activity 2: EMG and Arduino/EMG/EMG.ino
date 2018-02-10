void setup() {
 Serial.begin(115200);   //Initialize Serial Communication
 }
 
void loop() {
   Serial.print(micros());  //print out time in seconds
   Serial.print("\t");
   Serial.println(analogRead(A0));  //Print out the raw EMG values into serial monitor
   delay(200);
}

