#include "JsonConversionHandler.h"
#include "../data/RxMessageQueue.h"

void JsonConversionHandler::registerCallback(const String &topic, Callback callback)
{
    callbacks[topic] = callback;
}

void JsonConversionHandler::loop()
{
    std::pair<String, String> data = RxMessageQueue::getInstance().pop();

    // 데이터가 비어 있으면 무시
    if (data.first == "" || data.second == "")
        return;

    // 콜백이 등록되어 있으면 호출
    auto it = callbacks.find(data.first);
    if (it != callbacks.end())
    {
        Callback &callback = it->second;
        callback(data.second);  // payload 전달
    }
    else
    {
        Serial.println("No callback registered for topic: " + data.first);
    }
}

