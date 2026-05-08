#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver(2000, 2, 9, 10); // RX on 2, TX on 9

void setup()
{
    Serial.begin(9600);   // Debugging only
    if (!driver.init()) Serial.println("init failed");
    pinMode(4, OUTPUT);
}

void loop()
{
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        String str = "";

        for (int i = 0; i < buflen; i++) {
            str += (char)buf[i];
        }
        
        Serial.println(str);
        
        if (str == "HIGH") { 
            digitalWrite(4, LOW);
        } else {
            digitalWrite(4, HIGH);
        }
    }
}