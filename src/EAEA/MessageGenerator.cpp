#include <iostream>
#include "FileManager.hpp"
#include "Cryptographer.hpp"

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
    Cryptographer cryptographer;
    std::string hash = cryptographer.hashMessage(message);
    std::string fileContent = username + "\n" + hash + "\n" + message;
    fileManager.saveMessage(fileContent, "/home/mariana.murilloquintana/EAEA/"+username);

    std::vector<std::string> messages = fileManager.searchForMessages();
    for(const auto& message : messages){
        std::cout << message << std::endl << std::endl;
    }
    return 0;
}