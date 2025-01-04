#include "server.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <vector>

Server::Server(int port) : serverPort(port), serverSocket(INVALID_SOCKET), clientSocket(INVALID_SOCKET), ctx(nullptr) {
    std::cout << "Server created and will work on port: " << serverPort << std::endl;
}

Server::~Server() {
    if (clientSocket != INVALID_SOCKET) {
        shutdown(clientSocket, SD_BOTH);
        closesocket(clientSocket);
    }

    if (serverSocket != INVALID_SOCKET) {
        shutdown(serverSocket, SD_BOTH);
        closesocket(serverSocket);
    }

    if (ctx != nullptr) {
        SSL_CTX_free(ctx);
    }

    WSACleanup();
    std::cout << "Server closed" << std::endl;
}

void Server::start() {
    init();
    while (true) {
        acceptClients();
    }
}

void Server::init() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        return;
    }

    // Создание сокета
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

    // Инициализация OpenSSL
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    if (SSL_library_init() < 0) {
        std::cerr << "SSL library initialization failed" << std::endl;
        return;
    }

    Crypto crypto;
    ctx = crypto.generatingKeys();  // Генерация ключей

    std::cout << "Server listening on port " << serverPort << std::endl;
}

void Server::acceptClients() {
    int clientLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        return;
    } else {
        std::cout << "Client connected" << std::endl;
    }

    // Создаем SSL-сессию
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, (int)clientSocket);

    if (SSL_accept(ssl) <= 0) {
        std::cerr << "SSL accept failed" << std::endl;
        SSL_free(ssl);
        shutdown(clientSocket, SD_BOTH);
        closesocket(clientSocket);
        return;
    } else {
        std::cout << "SSL all good" << std::endl;
    }

    // Обработка клиента
    handleClient(ssl);

    // Освобождение SSL-сессии
    SSL_free(ssl);
    shutdown(clientSocket, SD_BOTH);
    closesocket(clientSocket);
}

void Server::handleClient(SSL* ssl) {\
    std::string clientMessage = receiveData(ssl);
    std::cout << "Received from client: " << clientMessage << std::endl;

    if (clientMessage == "new") {
        sendCACertificateToClient(ssl);
    }  
}

std::string Server::receiveData(SSL* ssl) {
    std::vector<char> receivedData;
    int bytesReceived;
    bytesReceived = SSL_read(ssl, buffer, sizeof(buffer) - 1);
    receivedData.insert(receivedData.end(), buffer, buffer + bytesReceived);
    receivedData.push_back('\0');
    return std::string(receivedData.begin(), receivedData.end() - 1);
}

void Server::sendCACertificateToClient(SSL* ssl) {
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

    // Преобразуем буфер в строку
    std::string dataStr(buffer.begin(), buffer.end());

    // Найдём позицию конца сертификата
    std::string endMarker = "-----END CERTIFICATE-----";
    size_t pos = dataStr.find(endMarker);

    if (pos != std::string::npos) {
        // Оставляем данные до конца строки "-----END CERTIFICATE-----" + перенос строки
        dataStr = dataStr.substr(0, pos + endMarker.length() + 1);
    }

    // Создаём const char* из строки
    const char* data = dataStr.c_str();
    std::cout << "DATA --- \t" << data << std::endl;
    int bytes_sent = 0;

    bytes_sent = SSL_write(ssl, data, strlen(data));

    if (bytes_sent <= 0) {
        int error_code = SSL_get_error(ssl, bytes_sent);
        std::cerr << "SSL_write error: " << error_code << std::endl;
    } else {
        std::cout << "Sent " << bytes_sent << " bytes." << std::endl;
    }
}
