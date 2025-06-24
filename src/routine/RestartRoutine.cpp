#include "RestartRoutine.h"
#include "../storage/NVSStorage.h"

RestartRoutine::RestartRoutine(uint8_t pin, unsigned long thresholdMillis)
    : _pin(pin), _thresholdMillis(thresholdMillis), _lowStartTime(0), _wasLow(false)
{
    pinMode(_pin, INPUT);
}

void RestartRoutine::checkRoutine()
{
    int state = digitalRead(_pin);

    if (state == LOW)
    {
        if (!_wasLow)
        {
            _lowStartTime = millis(); // Mark time when pin goes LOW
            _wasLow = true;
        }
        else
        {
            if (millis() - _lowStartTime >= _thresholdMillis)
            {
                Serial.println("EEPROM data will be erased. Performing factory reset...");
                for(int i=0; i<10; i++)
                {
                    digitalWrite(LED_BUILTIN, LOW);
                    delay(100);
                    digitalWrite(LED_BUILTIN, HIGH);
                    delay(100);
                }
                NVSStorage::getInstance().reset(); // Erase EEPROM
                delay(100);             // Ensure EEPROM is flushed
                ESP.restart();          // Restart device
                _wasLow = false;        // Reset flag (will not matter due to restart)
            }
        }
    }
    else
    {
        _wasLow = false; // Reset state if pin is HIGH
    }
}
