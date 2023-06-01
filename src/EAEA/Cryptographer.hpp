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
    Cryptographer() {
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();
    }

    ~Cryptographer() {
        EVP_cleanup();
        ERR_free_strings();
    }

    std::string signMessage(const std::string& user, const std::string& message, const std::string& publicKeyFilePath) {
        // TODO: Define separator character
        return user + "\n" + this->hashMessage(message) + "\n" + message;
    }

    bool validHash(const std::string& receivedHash, const std::string& message) {
        std::string hashedMessage = this->hashMessage(message);
        if (receivedHash.compare(hashedMessage) == 0) {
            return true;
        }
        return false;
    }

    std::string hashMessage(const std::string& message) {
        // Declara una variable para almacenar el hash SHA-256
        unsigned char hash[SHA256_DIGEST_LENGTH];
        // Inicializa el contexto SHA-256
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        // Actualiza el contexto con el mensaje
        SHA256_Update(&sha256, message.c_str(), message.size());
        // Finaliza el cálculo del hash y lo almacena en la variable hash
        SHA256_Final(hash, &sha256);
        // Convierte el hash a una cadena hexadecimal
        std::string sha256Hash;
        char hexChar[3];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            sprintf(hexChar, "%02x", hash[i]);
            sha256Hash += hexChar;
        }
        // Devuelve el hash SHA-256 como una cadena de caracteres
        return sha256Hash;
        return message;
    }

    std::string encrypt(const std::string& message, const std::string& publicKeyFilePath) {
        FILE* publicKeyFile = fopen(publicKeyFilePath.c_str(), "rb");
        if (!publicKeyFile) {
            throw std::runtime_error("Failed to open public key file");
        }

        EVP_PKEY* publicKey = PEM_read_PUBKEY(publicKeyFile, nullptr, nullptr, nullptr);
        fclose(publicKeyFile);

        if (!publicKey) {
            throw std::runtime_error("Failed to read public key");
        }

        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(publicKey, nullptr);
        if (!ctx) {
            EVP_PKEY_free(publicKey);
            throw std::runtime_error("Failed to create EVP_PKEY_CTX");
        }

        if (EVP_PKEY_encrypt_init(ctx) <= 0) {
            EVP_PKEY_free(publicKey);
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize encryption");
        }

        size_t encryptedLength = EVP_PKEY_size(publicKey);
        std::string encrypted(encryptedLength, '\0');

        if (EVP_PKEY_encrypt(ctx, reinterpret_cast<unsigned char*>(&encrypted[0]), &encryptedLength,
                             reinterpret_cast<const unsigned char*>(message.c_str()), message.length()) <= 0) {
            EVP_PKEY_free(publicKey);
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Encryption failed");
        }

        EVP_PKEY_free(publicKey);
        EVP_PKEY_CTX_free(ctx);

        encrypted.resize(encryptedLength);
        return encrypted;
    }

    std::string decrypt(const std::vector<unsigned char>& message, const std::string& privateKeyFilePath, const std::string& passphrase = "") {
        FILE* privateKeyFile = fopen(privateKeyFilePath.c_str(), "rb");
        if (!privateKeyFile) {
            throw std::runtime_error("Failed to open private key file");
        }

        EVP_PKEY* privateKey = PEM_read_PrivateKey(privateKeyFile, nullptr, nullptr,
                                                const_cast<char*>(passphrase.c_str()));
        fclose(privateKeyFile);

        if (!privateKey) {
            throw std::runtime_error("Failed to read private key");
        }

        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(privateKey, nullptr);
        if (!ctx) {
            EVP_PKEY_free(privateKey);
            throw std::runtime_error("Failed to create EVP_PKEY_CTX");
        }

        if (EVP_PKEY_decrypt_init(ctx) <= 0) {
            EVP_PKEY_free(privateKey);
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize decryption");
        }

        size_t decryptedLength = message.size();
        std::string decrypted(decryptedLength, '\0');

        if (EVP_PKEY_decrypt(ctx, reinterpret_cast<unsigned char*>(&decrypted[0]), &decryptedLength,
                            message.data(), message.size()) <= 0) {
            EVP_PKEY_free(privateKey);
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("\tDecryption failed");
        }

        EVP_PKEY_free(privateKey);
        EVP_PKEY_CTX_free(ctx);

        decrypted.resize(decryptedLength);
        return decrypted;
    }
};
