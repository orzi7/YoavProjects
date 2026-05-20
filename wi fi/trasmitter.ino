#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

const int Xport = A1;

int Xvalue;
int rawXvalue;

const int middleJoystick = 512;
const int deadzone = 50;

const int minSpeed = 1100;
const int maxSpeed = 1900;

int speedValue;

RH_ASK driver(2000, 9, 2, 10); // RX on 9, TX on 2

void setup()
{
    Serial.begin(9600);   // Debugging only
    if (!driver.init()) Serial.println("init failed");
    pinMode(4, INPUT);
    pinMode(13, OUTPUT);
}

void loop()
{
    //const char *msg = "NA";

    rawXvalue = analogRead(Xport);

    if (rawXvalue < (middleJoystick - deadzone)) {
      speedValue = map(rawXvalue, middleJoystick - deadzone, 0, minSpeed, maxSpeed);
    } else if (rawXvalue > (middleJoystick + deadzone)) {
      speedValue = map(rawXvalue, middleJoystick + deadzone, 1023, minSpeed, maxSpeed);
    } else {
      speedValue = minSpeed;
    }
    digitalWrite(13, HIGH);

    if (speedValue < minSpeed) {
      speedValue = minSpeed;
    }

    if (speedValue > maxSpeed) {
      speedValue = maxSpeed;
    }

    if (digitalRead(4) == LOW) {
      speedValue = 0;
    }

    String msgString = String(speedValue); 
    const char *msg = msgString.c_str();


    Serial.print("Sending: ");
    Serial.println(msg);

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
}
