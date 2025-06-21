#include "MainRoutine.h"
#include "communication/TimeManager.h"

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
        timeManager_result = TimeManager::getInstance().sync();
        tuyaMQTTClient.begin(espClient, mqtt_broker, mqtt_port);
        tuyaMQTTClient_result = tuyaMQTTClient.connect();
    }
    else
    {
        Serial.println("WIFI connect fail!!");
    }

    Serial.printf("setup done wifiManager[%d] timeManager[%d] tuyaMQTTClient[%d]\n", wifiManager_result, timeManager_result, tuyaMQTTClient_result);
    controlRoutine.begin();
    conversionRoutine.begin();
    irDecoder.setup();
}

void MainRoutine::do1msTasks()
{
    unsigned long now = millis();
    if (now - prev1ms >= 1)
    {
        prev1ms = now;
        tuyaMQTTClient.loop();
        conversionRoutine.loop();
    }
}

void MainRoutine::do10msTasks()
{
    unsigned long now = millis();
    if (now - prev10ms >= 10)
    {
        prev10ms = now;
        controlRoutine.loopByIr(irDecoder.getCommand());
    }
}

void MainRoutine::do50msTasks()
{
    unsigned long now = millis();
    if (now - prev50ms >= 50)
    {
        prev50ms = now;
        controlRoutine.loopLed();
    }
}

void MainRoutine::do100msTasks()
{
    unsigned long now = millis();
    if (now - prev100ms >= 100)
    {
        prev100ms = now;
        restartRoutine.checkRoutine();
        tuyaMQTTClient.connect();
        controlRoutine.updatePos();
        controlRoutine.loopByApp();
    }
}

void MainRoutine::do1mTasks()
{
    unsigned long now = millis();
    if (now - prev1m >= 60 * 1000)
    {
        prev1m = now;
        controlRoutine.loopAlram();
        controlRoutine.loopSnooze();
    }
}