#pragma once

class Message;

class Subscriber {
public:
    virtual void Notify(Message& message) = 0;
};