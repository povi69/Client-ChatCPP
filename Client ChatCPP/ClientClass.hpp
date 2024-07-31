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

/**
 * @class client
 * @brief Handles client-side socket communication.
 */
class client
{
public:
    /**
     * @brief Sends a welcome message.
     */
    void welcomeMessage();

    /**
     * @brief Constructor. Initializes the client object.
     */
    client();

    /**
     * @brief Destructor. Cleans up resources.
     */
    ~client();

    /**
     * @brief Asks the user for their name and returns it.
     * @return The user's name as a std::string.
     */
    std::string setName();

    /**
     * @brief Initializes the Winsock library.
     */
    void initializeWinsock();

    /**
     * @brief Creates a socket.
     */
    void createSocket();

    /**
     * @brief Connects the socket to the server.
     * @param serverAddress The server's address as a C-string.
     */
    void connectToServer(const char* serverAddress);

    /**
     * @brief Handles sending messages to the server.
     * @param message The message to send as a C-string.
     */
    void sendMessages(const char* message);

    /**
     * @brief Manages the Protocol Data Unit (PDU) for the client.
     * @param clientMessage The message from the client.
     * @param name The name of the client.
     */
    void mangePDU(const char* clientMessage, std::string name);

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
    SOCKET _client_fd; ///< The client socket descriptor.
    sockaddr_in _serverAddr; ///< The server address structure.
    std::string name; ///< The name of the client.
};

#endif
