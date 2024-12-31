#include "server.h"
#include "crypto/crypto.h"
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/asn1.h>
#include <openssl/ssl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>

// Конструктор с параметрами
Server::Server(int port) {
    this->serverPort = port;
    std::cout << "Server created and would work on - " << serverPort << " port" << std::endl;
}


Server::~Server() {

    shutdown(serverSocket, SD_BOTH);
    WSACleanup();
    std::cout << "Server closed" << std::endl;
}

void Server::server_start() {
    init();
    while (true) {
        
        AcceptClients();
        
    }
    
}


void Server::init() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // Настройка адреса и порта
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        closesocket(serverSocket);
        WSACleanup();
        return;
    } 

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    Crypto crypto;
    ctx = crypto.generatingKeys();

    std::cout << "Server listening on port " << serverPort << std::endl;
}


void Server::AcceptClients() { 
    int clientLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        return;
    }
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, (int)clientSocket);

    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        std::cout << "SSL connection established with client." << std::endl;
        std::string recieving_string = handelClient();
        std::cout << "Server get - \t" << recieving_string << std::endl;
    }

    SSL_free(ssl);
    shutdown(clientSocket, SD_BOTH);
}



std::string Server::handelClient() { 
    std::vector<char> receivedData;

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived == 0) {
            break;
        }
        for (int i = 0; i < bytesReceived; ++i) {
            receivedData.push_back(buffer[i]);
        }
    }

    receivedData.push_back('\0');
    std::string receivedString(receivedData.begin(), receivedData.end() - 1);
    return receivedString;
    
}