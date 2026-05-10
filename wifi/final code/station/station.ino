
#include "transmitter.h"
#include "joystickModule.h"

int minSpeed = 1100;
int maxSpeed = 1700;

transmitter myTransmitter(2);
joystickModule myJoystick(A0, A2, 8, minSpeed, maxSpeed);

void setup() {
  // put your setup code here, to run once:
  myTransmitter.init();
  myJoystick.init();
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  // myTransmitter.writeData(myJoystick.getMappedXPose(), myJoystick.getMappedYPose());
  myTransmitter.writeData(myJoystick.getMappedXPose(), ",", myJoystick.getMappedYPose(), ",", myJoystick.getSW());
  delay(10);
}
