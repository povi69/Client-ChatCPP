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

client::client()
{

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
        exit(1);
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
        WSACleanup();
        exit(1);
    }
    else
    {
        std::cout << "Socket created Successfully\n";
    }
}

void client::connectToServer(const char* serverAddress) 
{
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, serverAddress, &_serverAddr.sin_addr);

    int connectResult = connect(_client_fd, (sockaddr*)&_serverAddr, sizeof(_serverAddr));
    if (connectResult == SOCKET_ERROR)
    {
        std::cout << "Failed to connect to server\n";
        closesocket(_client_fd);
        WSACleanup();
        exit(1);
    }
    else
    {
        std::cout << "Connected to server\n";
    }
}

void client::sendMessages(const char* message) 
{
    
    int sendResult = send(_client_fd, message, strlen(message), 0);
    if (sendResult == SOCKET_ERROR)
    {
        std::cout << "Failed to send message\n";
        closesocket(_client_fd);
        WSACleanup();
        exit(1);
    }
    else 
    {
        std::cout << "Message sent\n";
    }
}

void client::mangePDU(const char* clientMessage,std::string name)
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
    while (true) 
    {
        std::getline(std::cin, message);
        if (message == "exit") 
        {
            break;
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
        else if (bytesReceived == 0) 
        {
            std::cout << "Connection closed by the server." << std::endl;
            break;
        }
        else 
        {
            break;
        }
    }
}
void client::Run()
{
    std::thread sendThread(&client::sendMessageThread, this);
    std::thread receiveThread(&client::receiveMessagesThread, this);

    sendThread.join();
    receiveThread.join();
}