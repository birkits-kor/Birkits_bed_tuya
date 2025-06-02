#ifndef CONVERSION_ROUTINE_H
#define CONVERSION_ROUTINE_H

#include <Arduino.h>
#include "../conversion/JsonConversionHandler.h"

class ConversionRoutine
{
public:

    void begin();
    void loop();

private:
    JsonConversionHandler jsonConversionHandler;
    void intfunc(const String &payload);
};

#endif // CONVERSION_ROUTINE_H
