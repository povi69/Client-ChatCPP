#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
constexpr int PORT = 12345;
constexpr int BUFFER = 1024;

class client
{
public:
    /// <summary>
    /// sends a welcome message
    /// </summary>
    void welcomeMessage();
    /// <summary>
    /// Constructor
    /// </summary>
    client();
    /// <summary>
    /// Deconstructor
    /// </summary>
    ~client();
    /// <summary>
    /// Initialize the winsock library
    /// </summary>
    /// <summary>
    /// Asks the user for his name and return it
    /// </summary>
    /// <returns></returns>
    std::string setName();
    
    void initializeWinsock();
    /// <summary>
    /// Creating a socket to the user's IP
    /// </summary>
    void createSocket();
    /// <summary>
    /// Connecting the socket to the server
    /// </summary>
    /// <param name="serverAddress"></param>
    void connectToServer(const char* serverAddress);
    /// <summary>
    /// handles the sending messages in the server
    /// </summary>
    /// <param name="message"></param>
    void sendMessages(const char* message);
    /// <summary>
    /// Receives the message from the user and print it in the server and to the other users
    /// </summary>
    void mangePDU(const char* clientMessage,std::string name);

    std::string getCurrentTime();
    /// <summary>
    /// Run the functions (I on purpose did capital R)
    /// </summary>
    void Run();
    /// <summary>
    /// Closes the server if it doesn't receive any bytes
    /// </summary>
    void receiveMessagesThread();
    /// <summary>
    /// Recieves the message
    /// </summary>
    void sendMessageThread();

private:
    SOCKET _client_fd;
    sockaddr_in _serverAddr;
    std::string name;
};

#endif // CLIENT_HPP