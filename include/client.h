#pragma once

#include <string>

#include <json.hpp>


class Message;

class Client {

public:

    Client(){}
    Client(int port);


    bool CreateSocket();

    void Connect(std::string serverIPAddress, int serverPort);
    void Disconnect();

    bool Connected(){return clientSocket > -1;}

    void SendRequest(std::string message);
    std::string ReceiveResonse();

    bool PollForResponse();

private:
    int clientSocket;
    int port;
    
};