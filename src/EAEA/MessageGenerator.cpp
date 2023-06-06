#include <iostream>
#include "FileManager.hpp"
#include <cstdlib>
std::string getCommandOutput(const std::string& command);
int main(){
    std::string username = "";
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);
    std::cout << std::endl;
    std::string message = "";
    std::cout << "Enter your message: ";
    std::getline(std::cin, message);
    std::cout << std::endl;
    FileManager fileManager;
    std::string certPath = "/home/fabian.gonzalezrojas/in-secure_ring/src/EAEA/ca/certs/"+username+".crt";
    std::string keyPath = "";
    std::cout << "Enter the path of your key: ";
    std::getline(std::cin, keyPath);
    std::cout << std::endl;
    //sign message
    std::string signature;
    std::string command = "echo -n " + message + " | openssl dgst -sha256 -sign " + keyPath + " -out /home/fabian.gonzalezrojas/in-secure_ring/src/EAEA/ca/private/firma.sha256";
    //std::cout << command <<std::endl;
    std::string fullCommand = "bash -c \"" + command + "\"";
    std::system(fullCommand.c_str());

    signature = fileManager.Read("/home/fabian.gonzalezrojas/in-secure_ring/src/EAEA/ca/private/firma.sha256");

    //std::cout << signature << std::endl;
    if (!signature.empty()) {
        std::string fileContent = username + "\n" + signature + "\n" + message;
        fileManager.saveMessage(fileContent);
        //std::cout << "Success: The message was sign: "<< signature << std::endl;
    } else {
        std::cout << "Error: Not able to sign the message. \n";
        return 1;
    }

    // std::string extractPubKey = "openssl x509 -pubkey -noout -in "+certPath+" > /home/fabian.gonzalezrojas/in-secure_ring/src/EAEA/ca/private/pubkey.pem";
    // std::system(extractPubKey.c_str());
    // std::string verifyCommand = "bash -c \"echo -n '" + message + "' | openssl dgst -sha256 -verify <(openssl rsa -pubin -inform PEM -in /home/fabian.gonzalezrojas/in-secure_ring/src/EAEA/ca/private/pubkey.pem) -signature /home/fabian.gonzalezrojas/in-secure_ring/src/EAEA/ca/private/firma.sha256\"";
    // std::cout << "\n\n" <<verifyCommand <<std::endl;
    // std::cout << "Verifying result: " << getCommandOutput(verifyCommand) << std::endl;

    return 0;
}

std::string getCommandOutput(const std::string& command) {
    std::string result;
    char buffer[128];
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Error al ejecutar el comando");
    }
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}