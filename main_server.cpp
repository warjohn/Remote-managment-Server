#include <iostream>
#include <fmt/core.h>
#include <openssl/ssl.h>
#include <cstring>
#include "server/server.h"
#include <cstdlib>

int main(int argc, char* argv[]){
    fmt::print("Hello World!\n");

   if (argc < 2) {
        std::cerr << "Error: Port argument is missing." << std::endl;
        return 1; // Завершаем программу с ошибкой
    }

    int port = atoi(argv[1]);
    if (port <= 0) {
        std::cerr << "Error: Invalid port number." << std::endl;
        return 1; // Завершаем программу с ошибкой
    }


    Server server(port);
    server.server_start();

    system("pause");
    return 0;
}
