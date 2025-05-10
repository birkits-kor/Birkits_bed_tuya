#include <Arduino.h>
#include "communication/ConfigPortal.h"
#include "communication/WiFiManager.h"
#include "communication/TimeManager.h"
#include "routine/RestartRoutine.h"


ConfigPortal configPortal;
WiFiManager wifiManager;
TimeManager timeManager;
RestartRoutine restartRoutine(0, 100);  // GPIO 0번 핀, 연속 5번 LOW 상태를 체크

void setup()
{
    Serial.begin(115200);
    Serial.println("Birkits Bed start!!");
    configPortal.begin();
    if(wifiManager.connect())
        Serial.println("WIFI connected!!");
    else
    {
        Serial.println("WIFI connect fail!!");
        while (1)
            delay(1000);        
    }
    timeManager.sync();
}

void loop()
{
    restartRoutine.checkRoutine();
    sleep(0.1);
}
