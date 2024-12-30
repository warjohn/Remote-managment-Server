#include "crypto.h"

#include <iostream>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/asn1.h>
#include <openssl/ssl.h>
#include <vector>

Crypto::Crypto() {
    std::cout << "Crypto created" << std::endl;
}

Crypto::~Crypto() {
    std::cout << "Crypto closed" << std::endl;
}

void Crypto::generatingKeys() {

    std::unique_ptr<X509, decltype(&::X509_free)> certificate = createCertificate();
    if (certificate == nullptr) {
        std::cerr << "Failed to create certificate" << std::endl;
        return;
    }
    std::cout << "Certificate created - \t" << certificate << std::endl;


    std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)> keys = generateKeyPair(bits);
    if (!setPublicKey(certificate.get(), keys.get())) {
        std::cerr << "Failed to setPublicKey" << std::endl;
        return;
    }

    if (!signCert(certificate.get(), keys.get(), EVP_sha256())) {
        std::cerr << "Failed to signCert" << std::endl;
        return;
    }
    
    if (!saveCertToPemFile(certificate.get())) {
        std::cerr << "Failed to saveCertToPemFile" << std::endl;
        return;
    }
    std::cout << "Certificate created - \t" << certificate << std::endl;
}   

std::unique_ptr<X509, decltype(&::X509_free)> Crypto::createCertificate() {
    std::unique_ptr<X509, decltype(&::X509_free)> certificate(X509_new(), ::X509_free);
    if (certificate == nullptr) {
        std::cerr << "Failed to create certificate" << std::endl;
    }
    return certificate;
}


std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)> Crypto::generateKeyPair(int32_t bits) {
    std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)> key(EVP_RSA_gen(bits), ::EVP_PKEY_free);
    return std::move(key);
}
///
bool Crypto::setPublicKey(X509* cert, EVP_PKEY* key) {
    return X509_set_pubkey(cert, key) == 1;
}

bool Crypto::saveCertToPemFile(X509* cert) {
    bool result = false;
    
    std::unique_ptr<BIO, decltype(&::BIO_free)> bio(BIO_new(BIO_s_file()), ::BIO_free);
    
    if (bio != nullptr) {
        if (BIO_write_filename(bio.get(), const_cast<char*>(filename.c_str())) > 0) {
            if (PEM_write_bio_X509(bio.get(), cert) == 1) {
                result = true;
            } else {
                std::cerr << "Error writing certificate to PEM file" << std::endl;
                return false;
            }
        } else {
            std::cerr << "Error binding BIO to file: " << filename << std::endl;
            return false;
        }
    } else {
        std::cerr << "Error creating BIO object" << std::endl;
        return false;
    }
    
    return result;
}

bool Crypto::signCert(X509* cert, EVP_PKEY* key, const EVP_MD* algo) {
    return X509_sign(cert, key, algo) != 0;
}