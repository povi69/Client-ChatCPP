#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS



class client
{
public:
    
    void welcomeMessage();

    
    client() = default;

    
    /**
     * @brief Destructor. Cleans up resources.
     */
    ~client();

    /**
     * @brief Asks the user for their name and returns it.
     * @return The user's name as a std::string.
     */
    std::string setName();

    void initializeWinsock();

    void createSocket();

    /**
     * @brief Connects the socket to the server.
     * @param serverAddress The server's address as a C-string.
     */
    void connectToServer(const std::string &serverAddress);

    /**
     * @brief Handles sending messages to the server.
     * @param message The message to send as a C-string.
     */
    void sendMessages(const std::string &message);

    /**
     * @brief Manages the Protocol Data Unit (PDU) for the client.
     * @param clientMessage The message from the client.
     * @param name The name of the client.
     */
    void mangePDU(std::string clientMessage, std::string &name);

    /**
     * @brief Gets the current time.
     * @return The current time as a std::string.
     */
    std::string getCurrentTime();

    /**
     * @brief Runs the main client functions.
     */
    void Run();

    /**
     * @brief Receives messages from the server.
     *        Closes the connection if no bytes are received.
     */
    void receiveMessagesThread();

    /**
     * @brief Sends messages to the server.
     */
    void sendMessageThread();

private:
    static constexpr auto PORT = 12345;
    static constexpr auto BUFFER = 1024;
    SOCKET _client_fd; ///< The client socket descriptor.
    sockaddr_in _serverAddr; ///< The server address structure.
    std::string _name; ///< The name of the client.
};

#endif
