#include <Arduino.h>
#include "config.h"
#include "communication/ConfigPortal.h"
#include "communication/WiFiManager.h"
#include "communication/TimeManager.h"
#include "communication/TuyaMQTTClient.h"
#include "routine/RestartRoutine.h"
#include "storage/NVSStorage.h"

ConfigPortal configPortal;
WiFiManager wifiManager;
TimeManager timeManager;
TuyaMQTTClient tuyaMQTTClient;
WiFiClientSecure espClient;
RestartRoutine restartRoutine(0, 5000);

void setup()
{
    Serial.begin(115200);
    NVSStorage::getInstance().begin(); // 초기화 필수
    Serial.println("Birkits Bed start!!");
    configPortal.begin();

    bool wifiManager_result = false;
    bool timeManager_result = false;
    bool tuyaMQTTClient_result = false;

    if (wifiManager.connect())
    {
        wifiManager_result = true;
        Serial.println("WIFI connected!!");
        timeManager_result = timeManager.sync();
        tuyaMQTTClient.begin(espClient, mqtt_broker, mqtt_port);
        tuyaMQTTClient_result = tuyaMQTTClient.connect();
    }
    else
        Serial.println("WIFI connect fail!!");

    Serial.printf("setup done wifiManager[%d] timeManager[%d] tuyaMQTTClient[%d]\n", wifiManager_result, timeManager_result, tuyaMQTTClient_result);
}

void loop()
{
    restartRoutine.checkRoutine();
    tuyaMQTTClient.connect();
    tuyaMQTTClient.loop();
    sleep(0.1);
}
