#include "ClientClass.hpp"

int main()
{
    client client;
    client.welcomeMessage();
    client.initializeWinsock();
    client.createSocket();
    client.connectToServer("172.20.10.2");
    client.Run();

    return 0;
}
