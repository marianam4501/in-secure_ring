#ifndef EAEA_HPP
#define EAEA_HPP

#include <syslog.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>
#include "Server.hpp"
#include "Client.hpp"
#include "FileManager.hpp"
#include <vector>
#include <cstdlib>

class EAEA {
  private:
    Server *server;
    Client *client;
    std::string type;
    FileManager fileManager;

  public:
    EAEA(std::string type, std::string serverIP, std::string clientIP) {
        // Sender
        if (type.compare("s") == 0) {
            this->client = new Client(clientIP);
            this->server = NULL;
        }
        if (type.compare("m") == 0) {
            this->client = new Client(clientIP);
            this->server = new Server(serverIP);
        }
        if (type.compare("r") == 0) {
            this->client = NULL;
            this->server = new Server(serverIP);
        }
        this->type = type;
    }

    ~EAEA() {
        if (this->server != NULL) {
            free(this->server);
        }
        if (this->client != NULL) {
            free(this->client);
        }
    }

    bool run() {
        bool isRunning = true;
        if (type.compare("s") == 0) {
            isRunning = this->send();
        }
        if (type.compare("m") == 0) {
            isRunning = this->resend();
        }
        if (type.compare("r") == 0) {
            isRunning = this->receive();
        }
        return isRunning;
    }

    void errorLog() {
        this->writeLog("Runtime error: relaunching");
    }

  private:
    const std::string extractPubKeyPt1 = "openssl x509 -pubkey -noout -in ";
    const std::string extractPubKeyPt2 = " > /home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/private/pubkey.pem";
    const std::string verifyCommandPt1 = "bash -c \"echo -n '";
    const std::string verifyCommandPt2 = "' | openssl dgst -sha256 -verify <(openssl rsa -pubin -inform PEM -in /home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/private/pubkey.pem) -signature /home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/private/firma.sha256\"";;

    bool send() {
        std::cout << "Send start\n";
        const bool stop = false;
        while (!stop) {
            try {
                std::vector<std::string> messages = fileManager.searchForMessages();
                for(const auto& message : messages){
                    if(!message.empty()){
                        std::vector<std::string> messageParts = fileManager.SplitMessageFile(message);
                        std::string certPath = "/home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/certs/"+messageParts.at(0)+".crt";
                        fileManager.Write(messageParts.at(2),"/home/mariana.murilloquintana/in-secure_ring/src/EAEA/ca/private/firma.sha256");
                        std::string extractPubKey = this->extractPubKeyPt1 + certPath + this->extractPubKeyPt2;
                        std::system(extractPubKey.c_str());
                        std::string verify = verifyCommandPt1 + messageParts.at(3) + verifyCommandPt2;
                        std::string verifyResult = getCommandOutput(verify);
                        if(verifyResult == "Verified OK"){
                            this->writeLog("The signature was verified and it is OK. The message remains intact.");
                            this->writeLog("Sending message from "+ messageParts.at(0)/*el usuario*/);
                            if(this->client->send(message) == 0){
                                this->writeLog("Message sended");
                                std::cout << "Sended: [" << message << "]\n";
                                std::cout << "Length: [" << message.length() << "]\n";
                                std::string last_msg_processed_path = "/home/mariana.murilloquintana/EAEA/" + messageParts.at(0) + "/000000.txt";
                                std::string last_msg_processed = fileManager.Read(last_msg_processed_path);
                                int file_count = std::stoi(last_msg_processed);
                                file_count++;
                                last_msg_processed = convertToZeroPaddedString(file_count);
                                fileManager.Write(last_msg_processed, last_msg_processed_path);
                            }
                        } else if (verifyResult == "Verification Failure"){
                            this->writeLog("The signature was verified and it is invalid. Message discarded.");
                            std::string last_msg_processed_path = "/home/mariana.murilloquintana/EAEA/" + messageParts.at(0) + "/000000.txt";
                            std::string last_msg_processed = fileManager.Read(last_msg_processed_path);
                            int file_count = std::stoi(last_msg_processed);
                            file_count++;
                            last_msg_processed = convertToZeroPaddedString(file_count);
                            fileManager.Write(last_msg_processed, last_msg_processed_path);
                        }
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "\t\tsendCDCD error" << std::endl;
                return false;
            }
        }
        std::cout << "Send End\n";
        return false;
    }

    bool resend() {
        std::cout << "Resend start\n";
        const bool stop = false;
        while (!stop) {
            try {
                std::vector<unsigned char> message = this->server->start();
                std::string received_message(message.begin(), message.end());
                this->client->send(received_message);
                std::cout << "Resend: [" << received_message << "]\n";
                this->writeLog("Message received and resended to next computer");
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "\t\tresendCDCD error" << std::endl;
                return false;
            }
        }
        std::cout << "Resend End\n";
        return false;
    }

    bool receive() {
        std::cout << "Receive start\n";
        const bool stop = false;
        while (!stop) {
            try {
                std::vector<unsigned char> message = this->server->start();
                std::string received_message(message.begin(), message.end());
                std::cout << "Length received: [" << received_message.length() << "]\n";
                this->writeLog("Message received");
                this->fileManager.saveMessage(received_message);
                this->writeLog("Message stored");
                std::cout << "Received: [" << received_message << "]\n";
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "\t\treceiveCDCD error" << std::endl;
                return false;
            }
        }
        std::cout << "Receive End\n";
        return false;
    }

    std::vector<std::string> split(const std::string& input, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(input);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            tokens.push_back(item);
        }
        return tokens;
    } 

    std::string convertToZeroPaddedString(int number)
    {
        std::string numberString = std::to_string(number);
        std::string zeroPaddedString = numberString;

        // Agregar ceros a la izquierda si es necesario
        while (zeroPaddedString.length() < 6)
        {
            zeroPaddedString = "0" + zeroPaddedString;
        }

        return zeroPaddedString;
    }

    void writeLog(const std::string message) {
        openlog("Program [EAEA] ", LOG_PID, LOG_LOCAL4);
        syslog(LOG_NOTICE, "%s", message.c_str());
        closelog();
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
};

#endif