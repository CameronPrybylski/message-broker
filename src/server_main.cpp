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

    return 0;
}