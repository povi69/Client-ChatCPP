#define _CRT_SECURE_NO_WARNINGS
#include "ClientClass.hpp"
#include <string>
#include <thread>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>


void client::welcomeMessage()
{
    std::cout << "Welcome to the Client\n";
}


client::~client()
{
    closesocket(_client_fd);
    WSACleanup();
}

std::string client::setName()
{
    std::cout << "enter your name: \n";
    std::cin >> _name;
    return _name;
}

void client::initializeWinsock()
{
    WSADATA ws;
    if (WSAStartup(WINSOCK_VERSION, &ws) == SOCKET_ERROR) {
        throw std::runtime_error("WSA Failed to Initialize");
    }
    else
    {
        std::cout << "WSA opened Successfully\n";
    }
}

void client::createSocket()
{
    _client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_client_fd == INVALID_SOCKET)
    {
        throw std::runtime_error("failed to Initialize socket");

    }
    else
    {
        std::cout << "Socket created Successfully\n";
    }
}

void client::connectToServer(const std::string& serverAddress) 
{
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, serverAddress.c_str(), &_serverAddr.sin_addr);

    int connectResult = connect(_client_fd, (sockaddr*)&_serverAddr, sizeof(_serverAddr));
    if (connectResult == SOCKET_ERROR)
    {
        closesocket(_client_fd);
        throw std::runtime_error("failed to connect to the server");
    }
    else
    {
        std::cout << "Connected to server\n";
    }
}

void client::sendMessages(const std::string &message) 
{
    // Create a buffer of the exact size of the message
    int messageLength = message.size();
    const char* messageBuffer = message.c_str();

    // Send the message
    int sendResult = send(_client_fd, messageBuffer, messageLength, 0);
    if (sendResult == SOCKET_ERROR)
    {
        closesocket(_client_fd);
        throw std::runtime_error("failed to send message");        
    }
    else 
    {
        std::cout << "Message sent\n"; 
    }
}


void client::mangePDU(std::string clientMessage,std::string &name)
{
    std::string outputMessage;
    outputMessage = getCurrentTime() + " " + name + ": "+ clientMessage + "\n";
    std::cout << outputMessage << std::endl;
    sendMessages(outputMessage.c_str());
}

std::string client::getCurrentTime()
{
    time_t realTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream timeStream;
    timeStream << std::put_time(localtime(&realTime), "%F %T");
    return timeStream.str();
}
void client::sendMessageThread() 
{
    std::string name;
    name = setName();
    std::string message;
    bool flag = false;
    constexpr auto exitMessage = "exit";
    while (flag)
    {
        std::getline(std::cin, message);
        if (message == exitMessage) 
        {
            flag = true;
        }
        mangePDU(message.c_str(),name);
    }
}


void client::receiveMessagesThread()
{
    std::vector<char> buffer(BUFFER);
    int bytesReceived;
    while (true)
    {
        // Resize buffer if needed
        buffer.resize(BUFFER);

        bytesReceived = recv(_client_fd, buffer.data(), BUFFER, 0);
        if (bytesReceived > 0)
        {
            // \0 the received data if it's less than the buffer size
            if (bytesReceived < BUFFER) {
                buffer[bytesReceived] = '\0';
            }

            std::cout << "Received: " << buffer.data() << std::endl;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed by the server." << std::endl;
            break;
        }
        else
        {
            std::cout << "Failed to receive data." << std::endl;
            break;
        }
    }
}

void client::Run()
{
    //threads for sending the receiving messages
    std::thread sendThread(&client::sendMessageThread, this);
    std::thread receiveThread(&client::receiveMessagesThread, this);

    //Join makes sure that the thread has completed its execution before the program exits.
    sendThread.join();
    receiveThread.join();
}
