#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include "crypto/crypto.h"  // Предполагается, что этот класс генерирует ключи и сертификаты

class Server {
public:
    // Конструктор с параметрами
    Server(int port);

    ~Server();

    void start();  // Запуск сервера

private:
    int serverPort;      
    SOCKET serverSocket;   
    SOCKET clientSocket; 
    SSL_CTX* ctx;         // Контекст SSL
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[4096];
    const char cert_path[12] = "keys/ca.crt";

    void init();  // Инициализация сервера
    void acceptClients();  // Принятие клиентов
    void handleClient(SSL* ssl);  // Обработка клиента
    void sendCACertificateToClient(SSL* ssl);  // Отправка сертификата CA клиенту
    std::string receiveData(SSL* ssl);  // Получение данных от клиента
    std::string prossecData();
};

#endif // SERVER_H
