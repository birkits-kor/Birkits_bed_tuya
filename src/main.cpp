#include <Arduino.h>
#include "web/ConfigPortal.h"


ConfigPortal configPortal;

void setup()
{
    Serial.begin(115200);
    Serial.println("Birkits Bed start!!");
    delay(1000);
    configPortal.begin();
}

void loop()
{
    Serial.println("@");
    sleep(10);
}
