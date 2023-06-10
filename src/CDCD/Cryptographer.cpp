#include "Cryptographer.hpp"

Cryptographer::Cryptographer() {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
}

Cryptographer::~Cryptographer() {
    EVP_cleanup();
    ERR_free_strings();
}

std::string Cryptographer::encrypt(const std::string& message, const std::string& publicKeyFilePath) {
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

std::string Cryptographer::decrypt(const std::vector<unsigned char>& message, const std::string& privateKeyFilePath, const std::string& passphrase) {
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
