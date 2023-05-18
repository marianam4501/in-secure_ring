#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <iostream>
#include <stdexcept>
#include <openssl/err.h>

class Cryptographer {
public:
    Cryptographer(){}

    Cryptographer(const std::string& key_filename, int key_type){
        if(key_type == 1){
            //public_key = loadPublicKey(key_filename);
            //std::cout << "Load public key success..." << std::endl;
        } else if (key_type == 2){
            private_key = loadPrivateKey(key_filename);
            std::cout << "Load private key success..." << std::endl;
        }
    }

    Cryptographer(const std::string& public_key_filename, const std::string& private_key_filename) {
        //public_key = loadPublicKey(public_key_filename);
        //std::cout << "Load public key success..." << std::endl;
        //private_key = loadPrivateKey(private_key_filename);
        //std::cout << "Load private key success..." << std::endl;
    }

    ~Cryptographer() {
        RSA_free(public_key);
        RSA_free(private_key);
    }

    // std::string encrypt(const std::string& message) {
    //     std::cout << "Encrypting this message: " << message << std::endl;
    //     RSA * pub_key = loadPublicKey("../public_key.pem");
    //     if (pub_key == NULL) {
    //         std::cout << "Error: failed to load public key" << std::endl;
    //         return "";
    //     }
    //     std::cout << "RSA_size: " << RSA_size(pub_key) << std::endl;
    //     unsigned char encrypted_message[RSA_size(pub_key)];
    //     std::cout << "About to encrypt a message..." << std::endl;
    //     int encrypted_message_length = RSA_public_encrypt(message.size(), reinterpret_cast<const unsigned char*>(message.c_str()), encrypted_message, pub_key, RSA_PKCS1_OAEP_PADDING);
    //     std::cout << "message lenght: " << encrypted_message_length << std::endl;
    //     RSA_free(pub_key);
    //     return std::string(reinterpret_cast<const char*>(encrypted_message), encrypted_message_length);
    // }

    std::string encrypt(const std::string& message, const std::string& publicKeyFilePath) {
        // Abrir el archivo de llave pública
        FILE* publicKeyFile = fopen(publicKeyFilePath.c_str(), "rb");
        if (!publicKeyFile) {
            printf("No se pudo abrir el archivo de llave pública\n");
            return "";
        }

        // Leer la llave pública desde el archivo
        RSA* publicKey = PEM_read_RSA_PUBKEY(publicKeyFile, nullptr, nullptr, nullptr);
        fclose(publicKeyFile);

        if (!publicKey) {
            printf("No se pudo leer la llave pública del archivo\n");
            return "";
        }

        // Crear un BIO de memoria para leer el mensaje
        BIO* messageBio = BIO_new_mem_buf(message.c_str(), (int)message.length());

        // Crear un BIO de memoria para escribir el mensaje cifrado
        BIO* ciphertextBio = BIO_new(BIO_s_mem());

        // Crear un cifrador RSA
        EVP_PKEY* publicKeyEvp = EVP_PKEY_new();
        EVP_PKEY_set1_RSA(publicKeyEvp, publicKey);

        EVP_CIPHER_CTX* cipherCtx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(cipherCtx, EVP_aes_256_cbc(), nullptr, nullptr, nullptr);
        EVP_PKEY_CTX* pkeyCtx = EVP_PKEY_CTX_new(publicKeyEvp, nullptr);
        EVP_PKEY_encrypt_init(pkeyCtx);

        // Cifrar el mensaje con la llave pública
        const size_t bufferSize = 4096;
        unsigned char buffer[bufferSize];

        int bytesRead;
        while ((bytesRead = BIO_read(messageBio, buffer, bufferSize)) > 0) {
            int bytesWritten;
            EVP_PKEY_encrypt(pkeyCtx, buffer, (size_t*)&bytesWritten, (const unsigned char*)message.c_str(), message.length());
            BIO_write(ciphertextBio, buffer, bytesWritten);
        }

        // Obtener el mensaje cifrado y su longitud
        BUF_MEM* ciphertextBuf = nullptr;
        BIO_get_mem_ptr(ciphertextBio, &ciphertextBuf);

        std::string ciphertext(ciphertextBuf->data, ciphertextBuf->length);

        // Limpiar los recursos
        EVP_PKEY_CTX_free(pkeyCtx);
        EVP_CIPHER_CTX_free(cipherCtx);
        BIO_free(messageBio);
        BIO_free(ciphertextBio);
        RSA_free(publicKey);

        // Verificar si hubo algún error durante el cifrado
        if (bytesRead < 0) {
            printf("Error durante el cifrado del mensaje\n");
            return "";
        }

        return ciphertext;
    }
    std::string decrypt(const std::string& message, const std::string& privateKey) {
        EVP_PKEY* pkey = NULL;
        EVP_PKEY_CTX* pkeyCtx = NULL;
        BIO* privateKeyBio = NULL;
        std::string decrypted;

        privateKeyBio = BIO_new_mem_buf(privateKey.c_str(), -1);
        if (privateKeyBio == NULL) {
            std::cout << "Error: BIO_new_mem_buf failed." << std::endl;
            return "";
        }

        pkey = PEM_read_bio_PrivateKey(privateKeyBio, NULL, NULL, NULL);
        if (pkey == NULL) {
            std::cout << "Error: PEM_read_bio_PrivateKey failed." << std::endl;
            BIO_free(privateKeyBio);
            return "";
        }

        pkeyCtx = EVP_PKEY_CTX_new(pkey, NULL);
        if (pkeyCtx == NULL) {
            std::cout << "Error: EVP_PKEY_CTX_new failed." << std::endl;
            EVP_PKEY_free(pkey);
            BIO_free(privateKeyBio);
            return "";
        }

        if (EVP_PKEY_decrypt_init(pkeyCtx) <= 0) {
            std::cout << "Error: EVP_PKEY_decrypt_init failed." << std::endl;
            EVP_PKEY_CTX_free(pkeyCtx);
            EVP_PKEY_free(pkey);
            BIO_free(privateKeyBio);
            return "";
        }

        size_t encryptedLength = message.length();
        size_t bufferLength = encryptedLength + EVP_PKEY_size(pkey) - 12; // Maxium ciphertext length
        unsigned char* buffer = new unsigned char[bufferLength];
        size_t bytesWritten = bufferLength;

        if (EVP_PKEY_decrypt(pkeyCtx, buffer, &bytesWritten, (const unsigned char*)message.c_str(), encryptedLength) <= 0) {
            char errorBuf[120];
            ERR_error_string(ERR_get_error(), errorBuf);
            std::cout << "Error: EVP_PKEY_decrypt failed. " << std::endl;
            delete[] buffer;
            EVP_PKEY_CTX_free(pkeyCtx);
            EVP_PKEY_free(pkey);
            BIO_free(privateKeyBio);
            return "";
        }

        decrypted = std::string((const char*)buffer, bytesWritten);
        delete[] buffer;
        EVP_PKEY_CTX_free(pkeyCtx);
        EVP_PKEY_free(pkey);
        BIO_free(privateKeyBio);

        return decrypted;
    }

private:
    RSA* public_key;
    RSA* private_key;

    RSA* loadPublicKey(const std::string& filename) {
        std::cout << "Loading public key... " << filename << std::endl;
        RSA* key = nullptr;
        FILE* file = fopen(filename.c_str(), "r");
        if (file) {
            std::cout << "File opened... "<< filename << std::endl;
            fseek(file, 0, SEEK_END);
            long size = ftell(file);
            fseek(file, 0, SEEK_SET);
            char *buffer = new char[size];
            fread(buffer, sizeof(char), size, file);
            //fclose(file);
            std::cout << "File contents: " << std::endl << buffer << std::endl;
            std::string myString = (std::string)buffer;
            delete[] buffer;
            key = PEM_read_RSA_PUBKEY(file, &key, nullptr, nullptr);
            //std::cout << "Just read public key... " << myString << std::endl;
            if (key == nullptr) {
                std::cout << "Error al leer la llave pública del archivo." << std::endl;
            }
            fclose(file);
            std::cout << "File closed... "<< filename << std::endl;
        }
        std::cout << "I just finished loading public key... Return " << public_key << std::endl;
        return key;
    }

    RSA* loadPrivateKey(const std::string& filename) {
        std::cout << "Loading private key..." << filename << std::endl;
        RSA* key = nullptr;
        FILE* file = fopen(filename.c_str(), "r");
        if (file) {
            std::cout << "File opened... "<< filename << std::endl;
            key = PEM_read_RSAPrivateKey(file, nullptr, nullptr, nullptr);
            std::cout << "Just read private key... " << std::endl;
            if (key == nullptr) {
                std::cout << "Error al leer la llave pública del archivo." << std::endl;
            }
            fclose(file);
            std::cout << "File closed... "<< filename << std::endl;
        }
        std::cout << "I just finished loading private key... Return " << private_key << std::endl;
        return key;
    }
};
