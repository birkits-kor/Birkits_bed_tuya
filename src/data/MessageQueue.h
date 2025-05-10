#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <Arduino.h>
#include <queue>

class MessageQueue
{
public:
    static MessageQueue &getInstance();

    void push(const String &topic, const String &message);
    bool isEmpty() const;
    std::pair<String, String> pop();
    std::pair<String, String> peek() const;

private:
    MessageQueue() = default;
    std::queue<std::pair<String, String>> queue;
};

#endif // MESSAGE_QUEUE_H
