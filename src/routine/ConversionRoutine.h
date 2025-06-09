#ifndef CONVERSION_ROUTINE_H
#define CONVERSION_ROUTINE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../conversion/JsonConversionHandler.h"
class ConversionRoutine
{
public:

    void begin();
    void loop();

private:
    JsonConversionHandler jsonConversionHandler;
    void intfunc(const String &payload);
    void speakerfunc(const String &payload);
    void lightfunc(const String &payload);
    void bedfunc(const String &payload);
    void stopfunc(const String &payload);
    void alarmfunc(const String &payload);

    String makeBedControlData(String topic);
    String makeLightControlData(String topic);
    String makeSpeakerSwData(String topic);
    String makeModeData(String topic);
    String makeAlarmData(String topic);
    String makeTimerData(String topic);
};

#endif // CONVERSION_ROUTINE_H
