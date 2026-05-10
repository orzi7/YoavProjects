#include "IMU.h"
#include "BLDC.h"
#include "LEDStrip.h"
#include "receiver.h"

// Instantiate your objects
IMU myIMU;
BLDC myMotors(9, 10); // Right motor on Pin 9, Left motor on Pin 10
LEDStrip myLEDs(5, 6, 11);
receiver myReceiver(2);

// Balancing Constants
const int midSpeed = 1500;
const float KP = 5.0;
const int TARGET_ANGLE = 0;

void setup() {
  myLEDs.init();
  Serial.begin(9600);
  myLEDs.setColor(255, 0, 0);
  delay(1000);
  
  // myIMU.init();
  // myMotors.init(); // Attaches servos and runs ESC calibration
  myReceiver.init();

  myLEDs.turnOff();
  delay(500);
  myLEDs.setColor(0, 255, 0);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(1000);
}

void loop() {
  String incomingData = myReceiver.readRawData();

  // float currentAngle = myIMU.getAngle();
  if (incomingData != "NA" && incomingData != "") {
    
    // 3. Parse the saved variable, NOT the radio buffer
    int XAxis = myReceiver.getValue(incomingData, ',', 0).toInt();
    int YAxis = myReceiver.getValue(incomingData, ',', 1).toInt();
    int switchState = myReceiver.getValue(incomingData, ',', 2).toInt();
    
    // Print them out nicely to verify
    Serial.print("X: ");
    Serial.print(XAxis);
    Serial.print(" | Y: ");
    Serial.print(YAxis);
    Serial.print(" | Switch: ");
    Serial.println(switchState);

    int redValue = map(XAxis, 1259, 1401, 0, 255);
    int greenValue = map(YAxis, 1138, 1401, 0, 255);

    myLEDs.setColor(redValue, greenValue, 0);
  }

  // myMotors.controlMotors(currentAngle, KP, TARGET_ANGLE, midSpeed);
}