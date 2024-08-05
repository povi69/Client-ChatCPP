#define _CRT_SECURE_NO_WARNINGS
#include "ClientClass.hpp"
#include <string>
#include <thread>
#include <iomanip>
#include <iostream>
#include <sstream>


void client::welcomeMessage()
{
    std::cout << "Welcome to the Client\n";
}

void client::exitServer()
{
    WSACleanup();
    exit(1);
}

client::~client()
{
    closesocket(_client_fd);
    WSACleanup();
}

std::string client::setName()
{
    std::cout << "enter your name: \n";
    std::cin >> name;
    return name;
}

void client::initializeWinsock()
{
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) == -1) {
        std::cout << "WSA Failed to Initialize\n";
        exitServer();
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
        std::cout << "Failed to create socket\n";
        exitServer();
    }
    else
    {
        std::cout << "Socket created Successfully\n";
    }
}

void client::connectToServer(const std::string serverAddress) 
{
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, serverAddress.c_str(), &_serverAddr.sin_addr);

    int connectResult = connect(_client_fd, (sockaddr*)&_serverAddr, sizeof(_serverAddr));
    if (connectResult == SOCKET_ERROR)
    {
        std::cout << "Failed to connect to server\n";
        closesocket(_client_fd);
        exitServer();
    }
    else
    {
        std::cout << "Connected to server\n";
    }
}

void client::sendMessages(const std::string message) 
{
    // Create a buffer of the exact size of the message
    int messageLength = message.size();
    const char* messageBuffer = message.c_str();

    // Send the message
    int sendResult = send(_client_fd, messageBuffer, messageLength, 0);
    if (sendResult == SOCKET_ERROR)
    {
        std::cout << "Failed to send message\n";
        closesocket(_client_fd);
        exitServer();
    }
    else 
    {
        std::cout << "Message sent\n"; 
    }
}


void client::mangePDU(std::string clientMessage,std::string name)
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
    while (!flag) 
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
    char buffer[BUFFER];
    int bytesReceived;
    while (true)
    {
        memset(buffer, 0, BUFFER);
        bytesReceived = recv(_client_fd, buffer, BUFFER, 0);
        if (bytesReceived > 0)
        {
            std::cout << "Received: " << buffer << std::endl;
        }
        else
        {
            std::cout << "Connection closed by the server." << std::endl;
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
