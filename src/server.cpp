#include <server.h>
#include <message_broker.h>


#include <iostream>
#include <cstring>       // memset
#include <sys/socket.h>  // bind, sockaddr
#include <netinet/in.h>  // sockaddr_in, INADDR_ANY, htons
#include <poll.h>        // pollfd
#include <unistd.h>

Server::Server(int port, MessageBroker &messageBroker) : port(port), messageBroker(&messageBroker), serverSocket(-1)
{
    
}

bool Server::Start()
{
    if(!(CreateSocket() && BindSocket() && ListenForConnections()))
    {
        return false;
    }
    while(true)
    {
        AcceptConnections();

        std::set<int>::iterator clientSocket = clientSockets.begin();
        for(; clientSocket != clientSockets.end();)
        {
            if(IsClientAvailable(*clientSocket))
            {
                if(!HandleClient(*clientSocket))
                {
                    clientSocket = clientSockets.erase(clientSocket);
                }
                else
                {
                    ++clientSocket;
                }
            }
            else
            {
                ++clientSocket;
            }
        }

        if(messageBroker->HasAvailableMessages())
        {
            SendResponse();
        }
    }
    return true;
}

void Server::Stop()
{

}

bool Server::IsClientAvailable(int clientSocket)
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

bool Server::HandleClient(int clientSocket)
{
    /*
        ssize_t recv(int socket, void *buffer, size_t length, int flags);
        - socket: The file descriptor of the socket to read from.
        - buffer: A pointer to the buffer where the received data should be stored.
        - length: The maximum number of bytes to receive (the size of the buffer).
        - flags: Modifiers that change the behavior of the call. 

        ssize_t send(int socket, const void *buffer, size_t length, int flags);
    */
    char buffer[1024];
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(bytesReceived > 0)
    {
        std::string message(buffer, bytesReceived);
        if(message == "disconnect")
        {
            return false;
        }

        nlohmann::json j;
        try
        {
            j = nlohmann::json::parse(message);
        }
        catch(nlohmann::json::parse_error& ex)
        {
            std::cerr << "Could not parse json: " << message << std::endl;
            return true;
        }
        
        messageBroker->HandleMessage(j, clientSocket);
        
    }
    return true;
}

bool Server::CreateSocket()
{
    /*
        int socket(int domain, int type, int protocol)
        domain:	Specifies the communications domain (protocol family). AF_INET: IPv4 Internet protocols
        type: Specifies the communication semantics. SOCK_STREAM: Sequenced, reliable, two-way, connection-based byte streams (TCP)
        protocol: 0: Automatically selects the default protocol for the given domain and type (e.g., TCP for SOCK_STREAM in AF_INET).
    */
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    return serverSocket != -1;
}

bool Server::BindSocket()
{
    /*
        int bind(int socket, const struct sockaddr *address, socklen_t address_len);
        - socket: The file descriptor of the socket to be bound (created via socket()).
        - address: A pointer to a socklen_t structure (such as sockaddr_in for IPv4 or sockaddr_in6 for IPv6) containing the local address to assign.
        - address_len: The size, in bytes, of the address structure pointed to by address.
    */
    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    address.sin_port = htons(this->port);       // Bind to server's port (converted to network byte order)
    if(bind(this->serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        return false;
    }
    return true;
}

bool Server::ListenForConnections()
{
    /*
        int listen(int sockfd, int backlog);
        sockfd: your listening socket.
        backlog: the maximum number of pending connections the system should queue.
    */
    return listen(this->serverSocket, 5) != -1;
}

bool Server::AcceptConnections()
{
    /*
        int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    */
    int clientSocket = accept(this->serverSocket, nullptr, nullptr);
    if(clientSocket >= 0)
        clientSockets.insert(clientSocket);
    return clientSocket != -1;
}

void Server::ReceiveRequest(nlohmann::json &jsonMessage)
{
    //messageBroker->HandleMessage(jsonMessage);
}

nlohmann::json Server::SendResponse()
{
    nlohmann::json j;
    std::unordered_map<int, std::vector<nlohmann::json>> messages = messageBroker->GetAvailableMessages();
    if(!messages.empty())
    {
        j = messages.begin()->second.front();
        std::cout << "Sent to " << messages.begin()->first << std::endl;
        std::unordered_map<int, std::vector<nlohmann::json>>::iterator subcriberMessages = messages.begin();
        for(; subcriberMessages != messages.end(); ++subcriberMessages)
        {
            SendMessagesToClient(subcriberMessages->first, subcriberMessages->second);
        }
    }
    return j;
}

void Server::SendMessagesToClient(int client, std::vector<nlohmann::json> messages)
{
    if(clientSockets.contains(client))
    {
        for(int i = 0; i < messages.size(); ++i)
        {
            std::string message = messages[i].dump();
            const char* response = message.c_str();
            ssize_t bytesSent = send(client, response, strlen(response), 0);
        }
    }
}
