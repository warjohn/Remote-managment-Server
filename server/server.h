#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/asn1.h>
#include <openssl/ssl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


class Server {
public:
    // Конструктор с параметрами
    Server(int serverPort);

    ~Server();

    void server_start ();

private:
    int serverPort;      
    SOCKET serverSocket;   
    SOCKET clientSocket; 
    SSL_CTX* ctx;
    char buffer[16384];
    struct sockaddr_in serverAddr, clientAddr;

    void init();
    void AcceptClients();
    std::string handelClient();
};

#endif // SERVER_H
