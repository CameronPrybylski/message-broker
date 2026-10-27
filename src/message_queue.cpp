#include <message_queue.h>

void MessageQueue::Enqueue(std::shared_ptr<Message> message)
{
    messageQueue.push(message);
}

std::shared_ptr<Message> MessageQueue::Dequeue()
{
    std::shared_ptr<Message> message = messageQueue.front();
    messageQueue.pop();
    return message;
}
