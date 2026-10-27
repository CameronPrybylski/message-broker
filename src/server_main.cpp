#include <iostream>
#include <filesystem>
#include <fstream>

#include <message_broker.h>
#include <server.h>



int main()
{
 
    MessageBroker messageBroker;
    messageBroker.RegisterQueue("hello");
    Server server(8080, messageBroker);
    

    bool success = server.Start();

    std::cout << success << std::endl;


    /*
    messageBroker.RegisterQueue("hello");
    

    nlohmann::json request;
    request["destination"] = "hello";
    request["type"] = "PUBLISH";
    server.ReceiveRequest(request);

    nlohmann::json request2;
    request2["destination"] = "hello";
    request2["type"] = "SUBSCRIBE";
    request2["consumer"] = "client";
    server.ReceiveRequest(request2);


    nlohmann::json response = server.SendResponse();

    if(response.contains("destination"))
    {
        std::cout << response["destination"] << " " << response["type"] << std::endl;
    }
    */
    return 0;
}