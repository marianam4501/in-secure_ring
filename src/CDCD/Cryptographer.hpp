#ifndef CRYPTOGRAPHER_HPP
#define CRYPTOGRAPHER_HPP

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Cryptographer {
  public:
    Cryptographer();

    ~Cryptographer();

    std::string encrypt(const std::string& message, const std::string& publicKeyFilePath);

    std::string decrypt(const std::vector<unsigned char>& message, const std::string& privateKeyFilePath, const std::string& passphrase = "");
};

#endif
