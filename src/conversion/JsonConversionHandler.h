#ifndef JSON_CONVERSION_HANDLER_H
#define JSON_CONVERSION_HANDLER_H

#include <map>
#include <functional>
#include <Arduino.h>

class JsonConversionHandler
{
public:
    using Callback = std::function<void(const String &)>;
    void registerCallback(const String &topic, Callback callback);
    void loop();

private:
    std::map<String, Callback> callbacks;
};

#endif
