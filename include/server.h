#pragma once

#include <string>
#include <set>
#include <map>

#include <json.hpp>

class MessageBroker;
class Message;

class Server {

public:
    Server(int port, MessageBroker& messageBroker);

    bool CreateSocket();
    bool BindSocket();
    bool ListenForConnections();
    bool AcceptConnections();

    bool Start();
    void Stop();

    bool IsClientAvailable(int clientSocket);
    bool HandleClient(int clientSocket);

    void DisconnectClient(int clientSocket);

    void ReceiveRequest(nlohmann::json& jsonMessage);
    nlohmann::json SendResponse();

    void SendMessagesToClient(int client, std::vector<nlohmann::json> messages);
    

private:
    MessageBroker* messageBroker;
    int port;
    int serverSocket;
    //int clientSocket;
    std::set<int> clientSockets;
    std::unordered_map<int, std::string> receiveBuffers;
};