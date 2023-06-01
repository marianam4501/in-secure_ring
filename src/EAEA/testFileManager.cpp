#include <iostream>
#include <vector>

#include "FileManager.hpp"

int main()
{
    std::string directoryPath = "/home/EAEA"; // Ruta al directorio base
    std::string filename = "000000.txt"; // Nombre del archivo a buscar
    std::vector<std::string> messages;
    FileManager fileManager;
    messages = fileManager.searchForMessages();

    for(const auto& message : messages){
        std::cout << "Message: " << message << std::endl;
    }

    fileManager.saveMessage("Sirva otra vez por favor","/home/mariana.murilloquintana/EAEA/fabian.gonzalezrojas");

    return 0;
}