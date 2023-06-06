#include <iostream>
#include "FileManager.hpp"
#include "Cryptographer.hpp"
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
    Cryptographer cryptographer;
    std::string certPath = "";
    std::cout << "Enter the path of your certificate: ";
    std::getline(std::cin, certPath);
    std::cout << std::endl;
    std::string keyPath = "";
    std::cout << "Enter the path of your key: ";
    std::getline(std::cin, keyPath);
    std::cout << std::endl;
    std::string successMessage = "Verified OK";
    std::string failureMessage = "Verification Failure";
    //sign message
    std::string signature;
    std::string command = "echo -n " + message + " | openssl dgst -sha256 -sign " + keyPath + " -out /home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/private/firma.sha256";
    std::string fullCommand = "bash -c \"" + command + "\"";
    std::system(fullCommand.c_str());

    signature = fileManager.Read("/home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/private/firma.sha256");

    std::cout << signature << std::endl;
    if (!signature.empty()) {
        std::string fileContent = username + "\n" + signature + "\n" + message;
        fileManager.saveMessage(fileContent, "/home/mariana.murilloquintana/EAEA/"+username);
        std::cout << "Success: The message was sign: "<< signature << std::endl;
    } else {
        std::cout << "Error: Not able to sign the message. \n";
        return 1;
    }

    //fileManager.ReadMessageFile("/home/mariana.murilloquintana/EAEA/mariana.murilloquintana/000034.txt");

    std::string verifyCommand = "echo -n "+message+" | openssl dgst -sha256 -sign "+keyPath+" -out firma.sha256";
    std::cout << "Verifying result: " << getCommandOutput(verifyCommand) << std::endl;

    return 0;
}//echo -n "STRING_A_FIRMAR" | openssl dgst -sha256 -sign privatekey.pem -out firma.sha256
//echo -n "STRING_A_FIRMAR" | openssl dgst -sha256 -sign /home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/mariana_key.pem -out firma.sha256
//echo -n "STRING_A_FIRMAR" | openssl dgst -sha256 -sign privatekey.pem -out firma.sha256
//echo -n "STRING_A_FIRMAR" | openssl dgst -sha256 -verify <(openssl x509 -pubkey -noout -in /home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/certs/mariana.crt) -signature firma.sha256
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

/*

int main() {
    std::string command = "ls -l"; // Ejemplo: ejecutar el comando 'ls -l'
    std::string output = getCommandOutput(command);
    std::cout << "Salida del comando: " << output << std::endl;
    
    return 0;
}
*/