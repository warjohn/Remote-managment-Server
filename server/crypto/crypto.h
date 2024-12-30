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

    void generatingKeys();

private:
    const uint32_t serialNum = 20;
    const long ver = 0x0;
    const int32_t bits = 2048;
    const std::string filename = "C:/Users/Menar/source/C++/socket/certificate.pem";

    std::unique_ptr<X509, decltype(&::X509_free)> createCertificate();
    std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)> generateKeyPair(int32_t bits);
    bool setPublicKey(X509* cert, EVP_PKEY* key);
    bool signCert(X509* cert, EVP_PKEY* key, const EVP_MD* algo);
    bool saveCertToPemFile(X509* cert);
    EVP_PKEY* loadPrivateKey(const std::string& keyFile);
    X509* loadCertificate(const std::string& certFile);

};

#endif // CRYPTO_H