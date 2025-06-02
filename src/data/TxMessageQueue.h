#ifndef TX_MESSAGE_QUEUE_H
#define TX_MESSAGE_QUEUE_H

#include <Arduino.h>
#include <queue>

class TxMessageQueue
{
public:
    static TxMessageQueue &getInstance();

    void push(const String &message);
    bool isEmpty() const;
    String pop();
    String peek() const;

private:
    TxMessageQueue() = default;
    std::queue<String> queue;
};

#endif // MESSAGE_QUEUE_H
