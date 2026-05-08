int state = 0;
const int Yport = A1;

int Yvalue;
int rawYvalue;

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(0, 1);

void setup() {
  BTSerial.begin(9600);
}

void loop() {
  rawYvalue = analogRead(Yport);
  Yvalue = map(rawYvalue, 0, 1023, 255, -255);

  state = Yvalue;

  Serial.println(state);

  BTSerial.println(state);
}
