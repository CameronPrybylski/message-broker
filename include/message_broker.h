#pragma once

#include <unordered_map>
#include <vector>

#include <json.hpp>

class QueueManager;
class Message;

class MessageBroker {
public:

    MessageBroker();

    void HandleMessage(nlohmann::json& jsonMessage, int client);

    void RegisterSubscriber(std::string queueName, int subscriber);
    void RegisterQueue(std::string queueName);

    void AddMessage(std::shared_ptr<Message> ptrMessage);

    bool HasAvailableMessages();
    std::unordered_map<int, std::vector<nlohmann::json>> GetAvailableMessages();

    void NotifySubscribers(std::string queueName, std::unordered_map<int, std::vector<nlohmann::json>>& subscriberMessages);

private:
    std::shared_ptr<QueueManager> queueManager;
    std::unordered_map<std::string, std::vector<int>> subscribers;

};