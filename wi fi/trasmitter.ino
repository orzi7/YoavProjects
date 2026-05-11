#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

int state = 0;
const int Yport = A0;

int Yvalue;
int rawYvalue;

const int middleJoystick = 430;
const int deadzone = 100;

const int minSpeed = 1100;
const int maxSpeed = 1900;

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
    const char *msg = "NA";

    rawYvalue = analogRead(Yport);
    if (rawYvalue < (middleJoystick - deadzone)) {
      msg = map(rawYvalue, middleJoystick - deadzone, 0, minSpeed, maxSpeed);
    } else if (rawYvalue > (middleJoystick + deadzone)) {
      msg = map(rawYvalue, middleJoystick + deadzone, 1023, minSpeed, maxSpeed);
    } else {
      msg = minSpeed;
    }
    digitalWrite(13, HIGH);

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
}
