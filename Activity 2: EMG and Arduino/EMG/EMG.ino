Arduino Code : 

#include "CurieIMU.h"
#include "CurieTimerOne.h"
float sample,scaled_sample,high_sample,low_sample,rect_sample,box_sample,elapsed_time;
int beginning;

void setup() {
 Serial.begin(250000);   //Initialize Serial Communication
 while(!Serial);       //Wait for connection
 CurieIMU.begin();
 CurieTimerOne.start(10000,EMG); //Create an interrupt function for taking EMG sample
 }
 
 float boxcarFilterSample(float sample)
{
  static const int boxcarWidth = 10; // Change this value to alter boxcar length
  static float recentSamples[boxcarWidth] = {0}; // hold onto recent samples
  static int readIndex = 0;              // the index of the current reading
  static float total = 0;                  // the running total
  static float average = 0;                // the average
  // subtract the last reading:
  total = total - recentSamples[readIndex];
  // add new sample to list (overwrite oldest sample)
  recentSamples[readIndex] = sample;
  // add the reading to the total:
  total = total + recentSamples[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= boxcarWidth) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  average = total / boxcarWidth;
  // send it to the computer as ASCII digits
  return average;
}

float thirdOrder_HIGH(float sample)
{
  static const float a[4] = {1.,         -2.37409474,  1.92935567, -0.53207537};
  static const float b[4] = { 0.72944072, -2.18832217,  2.18832217, -0.72944072};
  // x array for holding recent inputs (newest input as index 0, delay of 1 at index 1, etc.
  static float x[4] = {0};
  // x array for holding recent inputs (newest input as index 0, delay of 1 at index 1, etc.
  static float y[4] = {0};
  x[0] = sample;
  // Calculate the output filtered signal based on a weighted sum of previous inputs/outputs
  y[0] = (b[0]*x[0]+b[1]*x[1]+b[2]*x[2]+b[3]*x[3])-(a[1]*y[1]+a[2]*y[2]+a[3]*y[3]);
  y[0] /= a[0];
  // Shift the input signals by one timestep to prepare for the next call to this function
  x[3] = x[2];
  x[2] = x[1];
  x[1] = x[0];
  // Shift the previously calculated output signals by one time step to prepare for the next call to this function
  y[3] = y[2];
  y[2] = y[1];
  y[1] = y[0];
  return y[0];
}

float thirdOrder_LOW(float sample)
{
  static const float a[4] = { 1.00000000e+00,  -2.77555756e-16,   3.33333333e-01,  -1.85037171e-17};
  static const float b[4] = {0.16666667,  0.5,         0.5,         0.16666667};
  // x array for holding recent inputs (newest input as index 0, delay of 1 at index 1, etc.
  static float x[4] = {0};
  // x array for holding recent inputs (newest input as index 0, delay of 1 at index 1, etc.
  static float y[4] = {0};
  x[0] = sample;
  // Calculate the output filtered signal based on a weighted sum of previous inputs/outputs
  y[0] = (b[0]*x[0]+b[1]*x[1]+b[2]*x[2]+b[3]*x[3])-(a[1]*y[1]+a[2]*y[2]+a[3]*y[3]);
  y[0] /= a[0];
  // Shift the input signals by one timestep to prepare for the next call to this function
  x[3] = x[2];
  x[2] = x[1];
  x[1] = x[0];
  // Shift the previously calculated output signals by one time step to prepare for the next call to this function
  y[3] = y[2];
  y[2] = y[1];
  y[1] = y[0];
  return y[0];
}

void loop() {
}

void EMG(){   //Interrupt for taking EMG sample
   sample = analogRead(A0);
   beginning = micros();
   scaled_sample = ((sample*(3.3/1024))-1.5)/3.6;
   high_sample = thirdOrder_HIGH(scaled_sample);
   low_sample = thirdOrder_LOW(high_sample);
   rect_sample = abs(low_sample);
   box_sample = boxcarFilterSample(rect_sample);
   elapsed_time = micros() - beginning;
  
   Serial.print(beginning);  //print out time in seconds (1)
   Serial.print("\t");
   Serial.print(sample);  //Print out the raw EMG values into serial monitor (2)
   Serial.print("\t");
   Serial.print(scaled_sample,10); //Print out EMG voltage values (3)
   Serial.print("\t");
   Serial.print(high_sample,10); //Print out output of high-pass filter (4)
   Serial.print("\t");
   Serial.print(low_sample,10); //Print out output of low-pass filter (5)
   Serial.print("\t");
   Serial.print(rect_sample,10); //Print out rectified signal (6)
   Serial.print("\t");
   Serial.print(box_sample,10);  //Print out output of boxcar filter (7)
   Serial.print("\t");
   Serial.println(elapsed_time); //Print out time since started filtering (8)
}

