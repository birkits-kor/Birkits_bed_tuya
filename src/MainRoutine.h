#ifndef MAIN_ROUTINE_H
#define MAIN_ROUTINE_H

#include <Arduino.h>
#include "config.h"
#include "communication/ConfigPortal.h"
#include "communication/WiFiManager.h"
#include "communication/TuyaMQTTClient.h"
#include "routine/RestartRoutine.h"
#include "routine/ControlRoutine.h"
#include "routine/ConversionRoutine.h"
#include "storage/NVSStorage.h"
#include "communication/IRDecoder.h"

class MainRoutine
{
public:
    void init();
    void do1msTasks();
    void do10msTasks();
    void do50msTasks();
    void do100msTasks();
    void do10sTasks();
    void do1mTasks();

private:
    ConfigPortal configPortal;
    WiFiManager wifiManager;
    TuyaMQTTClient tuyaMQTTClient;
    WiFiClientSecure espClient;
    RestartRoutine restartRoutine = RestartRoutine(0, 5000);
    ControlRoutine controlRoutine;
    ConversionRoutine conversionRoutine;
    IRDecoder irDecoder;

    const int mqtt_port = 8883;                // MQTT port (TLS)
    const char *mqtt_broker = "m1.tuyaus.com"; // EMQX broker endpoint

    unsigned long prev1ms = 0;
    unsigned long prev10ms = 0;
    unsigned long prev50ms = 0;
    unsigned long prev100ms = 0;
    unsigned long prev10s = 0;
    unsigned long prev1m = 0;
};

#endif
