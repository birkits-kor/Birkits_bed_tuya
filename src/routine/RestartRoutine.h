#ifndef RESTARTROUTINE_H
#define RESTARTROUTINE_H

#include <Arduino.h>
#define LED_BUILTIN 2
class RestartRoutine
{
public:
    RestartRoutine(uint8_t pin, unsigned long thresholdMillis);
    void checkRoutine();

private:
    uint8_t _pin;
    unsigned long _thresholdMillis;
    unsigned long _lowStartTime;
    bool _wasLow;
};

#endif
