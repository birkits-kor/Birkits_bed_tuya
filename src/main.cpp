#include <Arduino.h>
#include "web/ConfigPortal.h"
#include "routine/RestartRoutine.h"


ConfigPortal configPortal;
RestartRoutine restartRoutine(0, 100);  // GPIO 0번 핀, 연속 5번 LOW 상태를 체크

void setup()
{
    Serial.begin(115200);
    Serial.println("Birkits Bed start!!");
    delay(1000);
    configPortal.begin();
}

void loop()
{
    restartRoutine.checkRoutine();
    sleep(0.1);
}
