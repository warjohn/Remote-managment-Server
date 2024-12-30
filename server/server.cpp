#include "server.h"
#include "crypto/crypto.h"

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>


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
    crypto.generatingKeys();

    std::cout << "Server listening on port " << serverPort << std::endl;
}


void Server::Createtlscerts() {

}