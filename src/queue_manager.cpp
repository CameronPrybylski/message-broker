#include <iostream>
#include <memory>

#include <queue_manager.h>
#include <message_queue.h>
#include <message.h>


void QueueManager::AddQueue(std::string queueName)
{
    if(messageQueues.count(queueName))
    {
        std::cerr << "Message Queue named: " << queueName << " already exists" << std::endl;
        return;
    }
    messageQueues[queueName] = std::make_shared<MessageQueue>();
}

void QueueManager::AddToQueue(std::shared_ptr<Message> message)
{
    std::string destination = message->GetDestination();
    if(messageQueues.contains(destination))
    {
        messageQueues[destination]->Enqueue(message);
    }
}

std::shared_ptr<Message> QueueManager::GetFromQueue(std::string queueName)
{
    if(messageQueues.contains(queueName))
    {
        return messageQueues[queueName]->Dequeue();
    }
    return nullptr;
}

std::vector<std::string> QueueManager::FindNonEmptyQueues()
{
    std::unordered_map<std::string, std::shared_ptr<MessageQueue>>::iterator queue = messageQueues.begin();
    std::vector<std::string> nonEmptyQueues;
    for(; queue != messageQueues.end(); ++queue)
    {
        if(!(queue->second->Empty()))
        {
            nonEmptyQueues.push_back(queue->first);
        }
    }
    return nonEmptyQueues;
}
