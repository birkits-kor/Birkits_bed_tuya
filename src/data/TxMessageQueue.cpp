#include "TxMessageQueue.h"

TxMessageQueue &TxMessageQueue::getInstance()
{
    static TxMessageQueue instance;
    return instance;
}

void TxMessageQueue::push(const String &message)
{
    queue.push(message);
}

bool TxMessageQueue::isEmpty() const
{
    return queue.empty();
}

String TxMessageQueue::pop()
{
    if (queue.empty())
        return "";
    auto front = queue.front();
    queue.pop();
    return front;
}

String TxMessageQueue::peek() const
{
    if (queue.empty())
        return "";
    return queue.front();
}
