#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

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
    
    if (digitalRead(4) == HIGH){
        msg = "HIGH";
        digitalWrite(13, HIGH);
    } else {
        msg = "LOW";
        digitalWrite(13, LOW);
    }

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    // delay(200);
}