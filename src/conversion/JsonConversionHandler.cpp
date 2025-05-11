#include "JsonConversionHandler.h"
#include "../data/MessageQueue.h"

void JsonConversionHandler::registerCallback(const String &topic, Callback callback)
{
    callbacks[topic] = callback;
}

void JsonConversionHandler::loop()
{
    std::pair<String, String> data = MessageQueue::getInstance().pop();
    if (data.first == "" || data.second == "")
        return;

    auto it = callbacks.find(data.first);
    if (it != callbacks.end())
        it->second(data.second);
    else
    {
        Serial.print("Error: No callback found for topic: ");
        Serial.println(data.first);
    }
}
