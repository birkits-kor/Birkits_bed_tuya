#include "MessageQueue.h"

MessageQueue &MessageQueue::getInstance()
{
    static MessageQueue instance;
    return instance;
}

void MessageQueue::push(const String &topic, const String &message)
{
    Serial.println(topic);
    Serial.println(message);
    
    queue.push({topic, message});
}

bool MessageQueue::isEmpty() const
{
    return queue.empty();
}

std::pair<String, String> MessageQueue::pop()
{
    if (queue.empty())
        return {"", ""};
    auto front = queue.front();
    queue.pop();
    return front;
}

std::pair<String, String> MessageQueue::peek() const
{
    if (queue.empty())
        return {"", ""};
    return queue.front();
}
