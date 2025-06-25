#include "MainRoutine.h"
#include "communication/TimeManager.h"

volatile bool wifiManager_result = false;
volatile bool tuyaMQTTClient_result = false;

void blinkTask(void *parameter)
{
    int val = 100;
    while (true)
    {
        if(wifiManager_result)
            val = 200;
        if(tuyaMQTTClient_result)
            val = 50; 
        analogWrite(LED_BUILTIN, val);
        vTaskDelay(50000 / val / portTICK_PERIOD_MS); // 500ms 대기
        analogWrite(LED_BUILTIN, 0);
        vTaskDelay(50000 / val / portTICK_PERIOD_MS);
    }

    // 혹시 루프에서 빠져나오면 태스크 삭제
    vTaskDelete(NULL);
}

void MainRoutine::init()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    NVSStorage::getInstance().begin(); // 초기화 필수
    Serial.println("Birkits Bed start!!");
    configPortal.begin();
    xTaskCreate(
        blinkTask,   // 태스크 함수
        "LED Blink", // 태스크 이름 (디버깅용)
        1024,        // 스택 크기 (단위: 워드)
        NULL,        // 파라미터
        10,           // 우선순위 (0~25, 높을수록 우선)
        NULL         // 태스크 핸들 (필요 없으면 NULL)
    );

    bool timeManager_result = false;

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
        controlRoutine.updatePos();
    }
}

void MainRoutine::do100msTasks()
{
    unsigned long now = millis();
    if (now - prev100ms >= 100)
    {
        prev100ms = now;
        restartRoutine.checkRoutine();
        tuyaMQTTClient_result = tuyaMQTTClient.connect();
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