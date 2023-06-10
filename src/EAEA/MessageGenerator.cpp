#include <iostream>
#include "FileManager.hpp"
#include <cstdlib>
#include <algorithm>

const std::string PATH_USER =  "mariana.murilloquintana";

std::string getCommandOutput(const std::string& command);

bool validUsername(std::string username);std::vector<std::string> usernames = {"manuel.arroyo", "rodrigo.piedra", "fabian.gonzalezrojas", 
        "cesar.lopezurena", "nayeri.azofeifa", "leonel.campos", "angie.castillocampos", "mariana.murilloquintana", 
        "jeremy.vargasartavia", "valery.murcia"};

int main(){
    std::string username = "";
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);
    std::cout << std::endl;
    if(validUsername(username)){
        std::string message = "";
        std::cout << "Enter your message: ";
        std::getline(std::cin, message);
        std::cout << std::endl;
        FileManager fileManager;
        std::string certPath = "/home/"+PATH_USER+"/in-secure_ring/src/EAEA/ca/certs/"+username+".crt";
        std::string keyPath = "";
        std::cout << "Enter the path of your key: ";
        std::getline(std::cin, keyPath);
        std::cout << std::endl;
        //sign message
        std::string signature;
        const std::string command = "echo -n " + message + " | openssl dgst -sha256 -sign " + keyPath + " -out /home/"+PATH_USER+"/in-secure_ring/src/EAEA/ca/private/firma.sha256";
        //std::cout << command <<std::endl;
        const std::string fullCommand = "bash -c \"" + command + "\"";
        std::system(fullCommand.c_str());

        signature = fileManager.Read("/home/"+PATH_USER+"/in-secure_ring/src/EAEA/ca/private/firma.sha256");

        //std::cout << signature << std::endl;
        if (!signature.empty()) {
            std::string fileContent = username + "\n" + signature + "\n" + message;
            fileManager.saveMessage(fileContent);
            //std::cout << "Success: The message was sign: "<< signature << std::endl;
        } else {
            std::cout << "Error: Not able to sign the message. \n";
            return 1;
        }
    } else {
        std::cout << "Error: username does not exist..." << std::endl;
    }

    return 0;
}

bool validUsername(std::string username){
    auto userFound = std::find(usernames.begin(), usernames.end(), username);
    return userFound != usernames.end();
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