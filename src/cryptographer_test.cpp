#include <iostream>
#include "cryptographer.hpp"
#include "FileManager.hpp"

int main() {

    // Cargar clave privada
    Cryptographer crypter("../public_key.pem", "../private_key.pem");

    // Mensaje a encriptar
    std::string message = "Hola mundo!";

    // Encriptar mensaje
    std::string encrypted_message = crypter.encrypt(message,"/home/mariana.murilloquintana/in-secure_ring/src/public_key.pem"); 

    // Desencriptar mensaje
    std::string decrypted_message = crypter.decrypt(encrypted_message,"/home/mariana.murilloquintana/in-secure_ring/src/private_key.pem");

    // Mostrar resultados
    std::cout << "Mensaje original: " << message << std::endl;
    //std::cout << "Mensaje encriptado: " << encrypted_message << std::endl;
    std::cout << "Mensaje desencriptado: " << decrypted_message << std::endl;
    FileManager::Write(encrypted_message, "encrypted_msg.txt");
    std::cout << "Mensaje encriptado: " << FileManager::Read("encrypted_msg.txt") << std::endl;
    FileManager::Write(decrypted_message, "decrypted_msg.txt");
    std::cout << "Mensaje desencriptado: " << FileManager::Read("decrypted_msg.txt") << std::endl;
    return 0;
}
