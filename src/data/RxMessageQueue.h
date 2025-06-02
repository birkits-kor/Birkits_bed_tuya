#ifndef RX_MESSAGE_QUEUE_H
#define RX_MESSAGE_QUEUE_H

#include <Arduino.h>
#include <queue>

class RxMessageQueue
{
public:
    static RxMessageQueue &getInstance();

    void push(const String &topic, const String &message);
    bool isEmpty() const;
    std::pair<String, String> pop();
    std::pair<String, String> peek() const;

private:
    RxMessageQueue() = default;
    std::queue<std::pair<String, String>> queue;
};

#endif // MESSAGE_QUEUE_H
