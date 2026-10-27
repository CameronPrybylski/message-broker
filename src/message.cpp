
#include <message.h>



Message::Message()
{
    this->timestamp = std::chrono::system_clock::now();
}

Message::Message(nlohmann::json j)
{
    this->timestamp = std::chrono::system_clock::now();
    this->destination = j["destination"];
    this->type = j["type"];
    if(j.contains("consumer"))
    {
        this->consumer = j["consumer"];
    }
    this->messageInJson = j;
}

Message::~Message()
{
}

void Message::SetJson(nlohmann::json j)
{
    if(j.contains("destination"))
    {
        this->destination = j["destination"];
    }
    this->messageInJson = j;
}
