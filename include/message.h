#pragma once

#include <chrono>
#include <json.hpp>

class Message {
public:
    Message();
    Message(nlohmann::json j);
    ~Message();

    void SetUniqueID(long id){this->uniqueID = id;}
    long GetUniqueID(){return this->uniqueID;}

    const std::string& GetDestination(){return this->destination;}
    const std::string& GetType(){return this->type;}
    const std::string& GetConsumer(){return this->consumer;}

    void SetConsumer(std::string consumer){this->consumer = consumer;}
    void SetJson(nlohmann::json j);
    nlohmann::json GetJson(){return this->messageInJson;}

    const std::chrono::time_point<std::chrono::system_clock>& GetTimeStamp(){return this->timestamp;}


private:
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    long uniqueID;

    std::string destination;
    std::string type;

    std::string consumer = "";

    nlohmann::json messageInJson;

};