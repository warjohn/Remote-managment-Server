#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <cstdint>
#include <openssl/x509.h>
#include <memory>

class Crypto {
public:
    Crypto();
    ~Crypto();

    SSL_CTX* generatingKeys();

private:

};

#endif // CRYPTO_H