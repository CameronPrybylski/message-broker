#pragma once

#include <unordered_map>
#include <vector>


class MessageQueue;
class Message;


class QueueManager {
public:

    void AddQueue(std::string queueName);
    
    void AddToQueue(std::shared_ptr<Message> message);
    std::shared_ptr<Message> GetFromQueue(std::string queueName);
    std::vector<std::string> FindNonEmptyQueues();

private:
    std::unordered_map<std::string, std::shared_ptr<MessageQueue>> messageQueues;
};