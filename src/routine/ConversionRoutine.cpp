#include "ConversionRoutine.h"

void ConversionRoutine::begin()
{
    jsonConversionHandler.registerCallback("init", [this](const String &payload) {
        this->intfunc(payload);
    });
}

void ConversionRoutine::loop()
{
    jsonConversionHandler.loop();
}

void ConversionRoutine::intfunc(const String &payload)
{
    Serial.println("[init] payload received: " + payload);
}