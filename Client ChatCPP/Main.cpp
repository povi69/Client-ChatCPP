#include "ClientClass.hpp"

int main()
{
    client client;
    client.initializeWinsock();
    client.createSocket();
    client.welcomeMessage();
    client.connectToServer("172.20.10.2");
    client.Run();
    return 0;
}
