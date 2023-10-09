// Include the AccelStepper Library
#include <AccelStepper.h>
// Include the SFR05 Library
#include "SRF05.h"

// Define motor interface type
#define motorInterfaceType 1

// Labview
String input;
float Speed_Goal_R = 0, Speed_Goal_L = 0, Speed_Actual_R = 0, Speed_Actual_L = 0;
float Acceleration = 0.4;
int time_start = 0, time_now = 0;

// Create an object for each Stepper
AccelStepper myStepperR(motorInterfaceType, 9, 8);
AccelStepper myStepperL(motorInterfaceType, 11, 10);

// Create a SRF05 sensor object
SRF05 SRF(3,4); // Trigger = Digital Pin 3, Echo = Digital Pin 4

void setup() {
  // Begin Serial communication
  Serial.begin(115200);

  // Left Stepper
	myStepperL.setMaxSpeed(1000);
	myStepperL.setSpeed(0);

  // Right Stepper
  myStepperR.setMaxSpeed(1000);
	myStepperR.setSpeed(0);

  // Ultrasonic sensor
  SRF.setCorrectionFactor(1.035);
}

void loop() {
  getDistance();
  readFromSerial();
  enforceAcceleration();
  setSpeed();
}

void getDistance(){
  time_now = millis();
  if(time_now - time_start > 50){
    Serial.println( SRF.getMillimeter() );
    time_start = millis();
  } 
}

void readFromSerial(){
  if (Serial.available()>0){
    while (Serial.available()) {
      char c = Serial.read();  //gets one byte from serial buffer
      input += c; //makes the String readString
      delay(2);  //slow looping to allow buffer to fill with next character
      if (c == ','){
        Speed_Goal_L = input.toInt();
        input = "";
      }
    }
    Speed_Goal_R = input.toInt();
    input = "";
  }
}

void setSpeed(){
  myStepperR.setSpeed(Speed_Actual_R);
  myStepperL.setSpeed(Speed_Actual_L);

  myStepperR.runSpeed();
	myStepperL.runSpeed();
}

void enforceAcceleration(){
  // Left Stepper
  if (Speed_Actual_L < Speed_Goal_L){
    Speed_Actual_L += Acceleration;
  }
  else if(Speed_Actual_L > Speed_Goal_L){
    Speed_Actual_L -= Acceleration;
  }
  else if(Speed_Actual_L == Speed_Goal_L){
    Speed_Actual_L = Speed_Goal_L;
  }

  // Right Stepper 
  if (Speed_Actual_R < Speed_Goal_R){
    Speed_Actual_R += Acceleration;
  }
  else if(Speed_Actual_R > Speed_Goal_R){
    Speed_Actual_R -= Acceleration;
  }
  else if(Speed_Actual_R == Speed_Goal_R){
    Speed_Actual_R = Speed_Goal_R;
  }
}