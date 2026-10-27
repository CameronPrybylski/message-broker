#include <client.h>

#include <iostream>

int main()
{
    Client client(5050);
    Client subscriber(5051);
    subscriber.CreateSocket();
    client.CreateSocket();
    subscriber.Connect("127.0.0.1", 8080);
    client.Connect("127.0.0.1", 8080);
    std::string jsonMessage = R"({
        "destination": "hello",
        "type": "PUBLISH",
        "content": "Hello, world!"
    })";
    std::string jsonMessage2 = R"({
        "destination": "hello",
        "type": "SUBSCRIBE"
    })";
    std::cout << "Sending request 2...\n";
    subscriber.SendRequest(jsonMessage2);
    std::cout << "Sending request 1...\n";
    client.SendRequest(jsonMessage);
    
    std::cout << "Receive response...\n";
    while(subscriber.Connected())
    {
        if(subscriber.PollForResponse())
        {
            std::string message = subscriber.ReceiveResonse();
            std::cout << "Response received...\n";
            std::cout << message << std::endl;
            break;
        }
    }
    
    return 0;
}