#include "MainRoutine.h"

void MainRoutine::init()
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
    {
        Serial.println("WIFI connect fail!!");
    }

    Serial.printf("setup done wifiManager[%d] timeManager[%d] tuyaMQTTClient[%d]\n", wifiManager_result, timeManager_result, tuyaMQTTClient_result);
    motorRoutine.begin();
}

void MainRoutine::do1msTasks()
{
    unsigned long now = millis();
    if (now - prev1ms >= 1)
    {
        prev1ms = now;
        // 1ms마다 실행할 작업
        tuyaMQTTClient.loop();
    }
}

void MainRoutine::do10msTasks()
{
    unsigned long now = millis();
    if (now - prev10ms >= 10)
    {
        prev10ms = now;
        // 10ms마다 실행할 작업
        // Serial.println(motorRoutine.getSensorValue("backrest"));
    }
}

void MainRoutine::do100msTasks()
{
    unsigned long now = millis();
    if (now - prev100ms >= 100)
    {
        prev100ms = now;
        // 100ms마다 실행할 작업
        restartRoutine.checkRoutine();
        tuyaMQTTClient.connect();
    }
}
