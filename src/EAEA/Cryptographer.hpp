#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/buffer.h>
#include <openssl/objects.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdio>

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

    std::string signMessage(const std::string& message, const std::string& certificatePath, const std::string& privateKeyPath) {
        // Cargar el certificado desde el archivo
        FILE* certificateFile = fopen(certificatePath.c_str(), "r");
        if (!certificateFile) {
            throw std::runtime_error("Error al abrir el archivo de certificado");
        }

        X509* certificate = PEM_read_X509(certificateFile, nullptr, nullptr, nullptr);
        if (!certificate) {
            fclose(certificateFile);
            throw std::runtime_error("Error al cargar el certificado");
        }
        fclose(certificateFile);

        // Cargar la clave privada desde el archivo
        FILE* privateKeyFile = fopen(privateKeyPath.c_str(), "r");
        if (!privateKeyFile) {
            X509_free(certificate);
            throw std::runtime_error("Error al abrir el archivo de clave privada");
        }

        EVP_PKEY* privateKey = PEM_read_PrivateKey(privateKeyFile, nullptr, nullptr, nullptr);
        if (!privateKey) {
            fclose(privateKeyFile);
            X509_free(certificate);
            throw std::runtime_error("Error al cargar la clave privada");
        }
        fclose(privateKeyFile);

        // Crear un objeto de contexto de firma
        EVP_MD_CTX* ctx = EVP_MD_CTX_create();
        if (!ctx) {
            EVP_PKEY_free(privateKey);
            X509_free(certificate);
            throw std::runtime_error("Error al crear el contexto de firma");
        }

        // Inicializar el objeto de contexto de firma
        if (EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, privateKey) != 1) {
            EVP_MD_CTX_destroy(ctx);
            EVP_PKEY_free(privateKey);
            X509_free(certificate);
            throw std::runtime_error("Error al inicializar el contexto de firma");
        }

        // Firmar el mensaje
        if (EVP_DigestSignUpdate(ctx, message.c_str(), message.size()) != 1) {
            EVP_MD_CTX_destroy(ctx);
            EVP_PKEY_free(privateKey);
            X509_free(certificate);
            throw std::runtime_error("Error al firmar el mensaje");
        }

        // Obtener la longitud de la firma
        size_t signatureLength;
        if (EVP_DigestSignFinal(ctx, nullptr, &signatureLength) != 1) {
            EVP_MD_CTX_destroy(ctx);
            EVP_PKEY_free(privateKey);
            X509_free(certificate);
            throw std::runtime_error("Error al obtener la longitud de la firma");
        }

        // Crear un búfer para almacenar la firma
        std::vector<unsigned char> signature(signatureLength);

        // Firmar y almacenar la firma en el búfer
        if (EVP_DigestSignFinal(ctx, signature.data(), &signatureLength) != 1) {
            EVP_MD_CTX_destroy(ctx);
            EVP_PKEY_free(privateKey);
            X509_free(certificate);
            throw std::runtime_error("Error al finalizar la firma");
        }

        // Liberar los recursos
        EVP_MD_CTX_destroy(ctx);
        EVP_PKEY_free(privateKey);
        X509_free(certificate);

        // Devolver la firma en formato base64
        BIO* bio = BIO_new(BIO_s_mem());
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, signature.data(), signatureLength);
        BIO_flush(bio);

        char* base64Data;
        size_t base64Length = BIO_get_mem_data(bio, &base64Data);

        std::string base64Signature(base64Data, base64Length);

        BIO_free(bio);

        return base64Signature;
    }

    std::string verifyAndExtractMessage(const std::string& signedMessage, const std::string& certificatePath) {
    // Cargar el certificado autofirmado de la autoridad certificadora
        X509* cert = nullptr;
        FILE* file = fopen(certificatePath.c_str(), "r");
        if (file) {
            cert = PEM_read_X509(file, nullptr, nullptr, nullptr);
            fclose(file);
        }
        if (!cert) {
            std::cerr << "Error al cargar el certificado autofirmado" << std::endl;
            return "";
        }

        // Crear un objeto BIO para leer el mensaje firmado
        BIO* bio = BIO_new(BIO_s_mem());
        BIO_puts(bio, signedMessage.c_str());

        // Crear un objeto PKCS7 para almacenar la firma
        PKCS7* pkcs7 = d2i_PKCS7_bio(bio, nullptr);
        BIO_free(bio);

        if (!pkcs7) {
            std::cerr << "Error al cargar la firma" << std::endl;
            X509_free(cert);
            return "";
        }

        // Verificar la firma digital
        int verifyResult = PKCS7_verify(pkcs7, nullptr, nullptr, nullptr, nullptr, 0);
        if (verifyResult <= 0) {
            std::cerr << "Error al verificar la firma" << std::endl;
            PKCS7_free(pkcs7);
            X509_free(cert);
            return "";
        }

        // Extraer el mensaje firmado
        BIO* payloadBio = PKCS7_dataDecode(pkcs7, nullptr, nullptr, nullptr);
        if (!payloadBio) {
            std::cerr << "Error al extraer el mensaje firmado" << std::endl;
            PKCS7_free(pkcs7);
            X509_free(cert);
            return "";
        }

        // Leer el mensaje desde el objeto BIO
        char buffer[4096];
        std::string extractedMessage;
        int bytesRead;
        while ((bytesRead = BIO_read(payloadBio, buffer, sizeof(buffer))) > 0) {
            extractedMessage.append(buffer, bytesRead);
        }

        // Liberar memoria y limpiar
        BIO_free(payloadBio);
        PKCS7_free(pkcs7);
        X509_free(cert);

        return extractedMessage;
    }
};
