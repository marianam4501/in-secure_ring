#include <iostream>
#include "Cryptographer.hpp"
#include "/home/mariana.murilloquintana/in-secure_ring/src/CDCD/FileManager.hpp"

int main() {
    Cryptographer cryptographer;

    // Mensaje a encriptar
    std::string message = "Qué felicidad.";

    // Encriptar mensaje
    std::string encrypted_message = cryptographer.encrypt(message,"public_key.pem"); 

    // Desencriptar mensaje
    std::string decrypted_message = cryptographer.decrypt(encrypted_message,"private_key.pem");

    // Mostrar resultados
    std::cout << "Mensaje original: " << message << std::endl;
    std::cout << "Mensaje encriptado: " << encrypted_message << std::endl;
    std::cout << "Mensaje desencriptado: " << decrypted_message << std::endl;
    //FileManager::Write(encrypted_message, "encrypted_msg.txt");
    //std::cout << "Mensaje encriptado: " << FileManager::Read("encrypted_msg.txt") << std::endl;
    //FileManager::Write(decrypted_message, "decrypted_msg.txt");
    //std::cout << "Mensaje desencriptado: " << FileManager::Read("decrypted_msg.txt") << std::endl;
    return 0;
}
