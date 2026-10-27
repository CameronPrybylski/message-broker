#include <message_broker.h>
#include <queue_manager.h>
#include <message.h>
#include <subscriber.h>

#include <iostream>

MessageBroker::MessageBroker()
{
    queueManager = std::make_shared<QueueManager>();
}

void MessageBroker::HandleMessage(nlohmann::json& jsonMessage, int client)
{
    std::shared_ptr<Message> message = std::make_shared<Message>(jsonMessage);
    if(message->GetType() == "PUBLISH")
    {
        AddMessage(message);
    }
    else if(message->GetType() == "SUBSCRIBE")
    {
        RegisterSubscriber(message->GetDestination(), client);
    }
    std::cout << "Handle message of type: " << message->GetType() << std::endl;
}

void MessageBroker::AddMessage(std::shared_ptr<Message> ptrMessage)
{
    queueManager->AddToQueue(ptrMessage);
}

Message* MessageBroker::GetMessage(std::string queueName)
{
    Message message = *(queueManager->GetFromQueue(queueName));
    //return NotifySubscribers(queueName, message);
    return nullptr;
}

bool MessageBroker::HasAvailableMessages()
{
    std::vector<std::string> nonEmptyQueues = queueManager->FindNonEmptyQueues();
    
    for(int i = 0; i < nonEmptyQueues.size(); ++i)
    {
        if(subscribers.contains(nonEmptyQueues[i]))
        {
            return true;
        }
    }
    return false;
}

std::unordered_map<int, std::vector<nlohmann::json>> MessageBroker::GetAvailableMessages()
{
    std::vector<std::string> nonEmptyQueues = queueManager->FindNonEmptyQueues();
    std::unordered_map<int, std::vector<nlohmann::json>> subscriberMessages;
    for(int i = 0; i < nonEmptyQueues.size(); ++i)
    {
        NotifySubscribers(nonEmptyQueues[i], subscriberMessages);
    }
    return subscriberMessages;
}

void MessageBroker::RegisterSubscriber(std::string queueName, int subscriber)
{
    std::cout << "Subscriber: " << subscriber << " subscribed to queue: " << queueName << std::endl;
    subscribers[queueName].push_back(subscriber);
}

void MessageBroker::RegisterQueue(std::string queueName)
{
    queueManager->AddQueue(queueName);
}

void MessageBroker::NotifySubscribers(std::string queueName, std::unordered_map<int, std::vector<nlohmann::json>>& subscriberMessages)
{
    if(subscribers.contains(queueName))
    {
        std::shared_ptr<Message> message = queueManager->GetFromQueue(queueName);
        nlohmann::json jsonMessage = message->GetJson();
        for(int i = 0; i < subscribers[queueName].size(); ++i)
        {
            subscriberMessages[subscribers[queueName][i]].push_back(jsonMessage);
        }
    }
    else 
    {
        std::cerr << "No subscribers to Queue named: " << queueName << std::endl;
    }
}
