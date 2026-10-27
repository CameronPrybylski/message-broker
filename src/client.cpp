#include <client.h>

#include <iostream>
#include <cstring>       // memset
#include <sys/socket.h>  // bind, sockaddr
#include <netinet/in.h>  // sockaddr_in, INADDR_ANY, htons
#include <arpa/inet.h>   // inet_pton()
#include <poll.h>        // poll
#include <unistd.h>

Client::Client(int port) : port(port), clientSocket(-1)
{

}

bool Client::CreateSocket()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    return clientSocket != -1;
}

void Client::Connect(std::string serverIPAddress, int serverPort)
{
    /*
        int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    */

    struct sockaddr_in address{};

    //IPv4 for AF_INET
    address.sin_family = AF_INET;

    //Convert string for ip address to usuable bytes for sockaddr_in
    const char* ip_addr = serverIPAddress.c_str();
    int success = inet_pton(AF_INET, ip_addr, &address.sin_addr);

    // Bind to server's port (converted to network byte order)
    address.sin_port = htons(serverPort);
    
    int connection = connect(clientSocket, (struct sockaddr*) &address, sizeof(address));

    if(connection == 0)
    {
        std::cout << "Connected!" << std::endl;
    }
    else{
        std::cout << "Not Connected!" << std::endl;
    }

}

void Client::Disconnect()
{
    std::string disconnect = "disconnect";
    const char* request = disconnect.c_str();
    send(clientSocket, request, strlen(request), 0);
    this->clientSocket = -1;
}

void Client::SendRequest(std::string message)
{
    const char* request = message.c_str();
    send(clientSocket, request, strlen(request), 0);
}

std::string Client::ReceiveResonse()
{
    char buffer[1024];
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(bytesReceived > 0)
    {
        std::string message(buffer, bytesReceived);
        nlohmann::json j;
        try {
            j = nlohmann::json::parse(message);
        } catch (nlohmann::json::exception& ex){
            std::cerr << ex.what() << '\n';
            return "ERROR";
        }

        for(auto item : j.items())
        {
            std::cout << item.key() << std::endl;
        }
        if(message == "PONG")
        {
            std::cout << "Client Received: " << message << std::endl;
        }
        return message;
    }
    return "";
}

bool Client::PollForResponse()
{
    struct pollfd pfd;
    pfd.fd = clientSocket;       // The socket you want to check
    pfd.events = POLLIN;      // Alert me when data is available to read
    pfd.revents = 0;          // Clear returned events

    // poll returns: > 0 if data is ready, 0 on timeout, -1 on error
    int timeout_ms = 1;
    int ret = poll(&pfd, 1, timeout_ms);
    
    if (ret > 0 && (pfd.revents & POLLIN)) {
        return true; 
    }
    return false;
}
