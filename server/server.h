#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>


class Server {
public:
    // Конструктор с параметрами
    Server(int serverPort);

    ~Server();

    void server_start ();

private:
    int serverPort;      
    int serverSocket;    
    struct sockaddr_in serverAddr, clientAddr;

    void init();

    void Createtlscerts();
    void CreateCAcerts();
    SSL_CTX* createSSLContext()
};

#endif // SERVER_H
