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
#include <fstream>


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
    } else {
        std::cerr << "Accept sucseful" << std::endl;
    }
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, (int)clientSocket);

    if (SSL_accept(ssl) <= 0) {
        std::cerr << "Error: SSL handshake failed." << std::endl;
        ERR_print_errors_fp(stderr);

        long verifyError = SSL_get_verify_result(ssl);
        if (verifyError == X509_V_ERR_CERT_HAS_EXPIRED || 
            verifyError == X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT || 
            verifyError == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY) {

            std::cout << "Client failed to provide a valid certificate." << std::endl;
            std::cout << "Do you want to send the CA certificate to the client? (yes/no): ";
            std::string response;
            std::cin >> response;

            if (response == "yes") {
                SendCACertificateToClient();
            } else {
                shutdown((int)clientSocket, SD_BOTH);
            }
        }

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


void Server::SendCACertificateToClient() {
    std::ifstream caFile("keys/ca.crt", std::ios::binary | std::ios::ate);
    if (!caFile.is_open()) {
        std::cerr << "Failed to open CA certificate file." << std::endl;
        return;
    }

    std::streamsize fileSize = caFile.tellg();
    caFile.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    if (!caFile.read(buffer.data(), fileSize)) {
        std::cerr << "Failed to read CA certificate file." << std::endl;
        return;
    }

    // Отправка CA сертификата клиенту
    if (send(clientSocket, buffer.data(), static_cast<int>(buffer.size()), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send CA certificate to client." << std::endl;
    } else {
        std::cout << "CA certificate sent to client." << std::endl;
    }
}