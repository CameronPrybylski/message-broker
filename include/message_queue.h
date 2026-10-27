#pragma once

#include <queue>

class Message;

class MessageQueue {

public:

    MessageQueue(){}


    void Enqueue(std::shared_ptr<Message> message);
    std::shared_ptr<Message> Dequeue();
    bool Empty(){return messageQueue.empty();}

    void SetQueueName(std::string name){this->queueName = name;}
    const std::string& GetQueueName(){return this->queueName;}

private:
    std::queue<std::shared_ptr<Message>> messageQueue;
    std::string queueName;

};