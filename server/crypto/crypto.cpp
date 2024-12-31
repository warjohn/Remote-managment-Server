#include "crypto.h"

#include <iostream>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/asn1.h>
#include <openssl/ssl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

Crypto::Crypto() {
    std::cout << "Crypto created" << std::endl;
    
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}

Crypto::~Crypto() {
    std::cout << "\n" << std::endl;
}


SSL_CTX* Crypto::generatingKeys() {
    const SSL_METHOD* method = TLS_server_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        std::cerr << "Unable to create SSL context." << std::endl;
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Загрузка сертификата и ключа
    if (SSL_CTX_use_certificate_file(ctx, "serverKeys/server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "serverKeys/server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if (!SSL_CTX_check_private_key(ctx)) {
        std::cerr << "Private key does not match the public certificate." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "SSL context initialized successfully." << std::endl;
    return ctx;
}