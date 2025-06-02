#include "RxMessageQueue.h"

RxMessageQueue &RxMessageQueue::getInstance()
{
    static RxMessageQueue instance;
    return instance;
}

void RxMessageQueue::push(const String &topic, const String &message)
{
    queue.push({topic, message});
}

bool RxMessageQueue::isEmpty() const
{
    return queue.empty();
}

std::pair<String, String> RxMessageQueue::pop()
{
    if (queue.empty())
        return {"", ""};
    auto front = queue.front();
    queue.pop();
    return front;
}

std::pair<String, String> RxMessageQueue::peek() const
{
    if (queue.empty())
        return {"", ""};
    return queue.front();
}
