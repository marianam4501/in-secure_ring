#ifndef CDCD_HPP
#define CDCD_HPP

#include <syslog.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "Server.hpp"
#include "Client.hpp"
#include "Cryptographer.hpp"
#include "FileManager.hpp"
#include "MessageGenerator.hpp"
#include <vector>

class CDCD {
  public:
    CDCD(std::string type, std::string serverIP, std::string clientIP) {
        // Sender
        if (type.compare("s") == 0) {
            this->client = new Client(serverIP, clientIP);
            this->server = NULL;
            this->cryptographer = new Cryptographer();
        }
        if (type.compare("m") == 0) {
            this->client = new Client(serverIP, clientIP);
            this->server = new Server(serverIP);
            this->cryptographer = NULL;
        }
        if (type.compare("r") == 0) {
            this->client = NULL;
            this->server = new Server(serverIP);
            this->cryptographer = new Cryptographer();
        }
        this->type = type;
    }

    ~CDCD() {
        if (this->server != NULL) {
            free(this->server);
        }
        if (this->client != NULL) {
            free(this->client);
        }
        if (this->cryptographer != NULL) {
            free(this->cryptographer);
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
    Server *server;
    Client *client;
    Cryptographer *cryptographer;
    std::string type;
    MessageGenerator generator;
    const std::string PATH_USER = "mariana.murilloquintana";
    
    bool send() {
        std::cout << "Send start\n";
        std::string message_count_path = "/home/"+PATH_USER+"/CDCD/000000.txt";
        std::string path = "/home/"+PATH_USER+"/CDCD/";
        const bool stop = false;
        while (!stop) {
            try {
                std::string last_msg_processed = FileManager::Read(message_count_path);
                std::string message = "";
                message = FileManager::Read(path+last_msg_processed+".txt");
                if(!message.empty()){
                    std::cout<<"Sending "<<last_msg_processed<<".txt..."<<std::endl;
                    this->writeLog("Sending message");
                    message = this->cryptographer->encrypt(message,"./src/public_key.pem"); 
                    if(this->client->send(message) == 0){
                        this->writeLog("Message sended");
                        std::cout << "Sended: [" << message << "]\nLength: [" << message.length() << "]\n";
                        int file_count = std::stoi(last_msg_processed);
                        file_count++;
                        last_msg_processed = convertToZeroPaddedString(file_count);
                        FileManager::Write(last_msg_processed, message_count_path);
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
                std::string decrypted_message = this->cryptographer->decrypt(message,"./src/private_key.pem");
                this->writeLog("Message received");
                generator.createMessage(decrypted_message);
                this->writeLog("Message stored");
                std::cout << "Received: [" << decrypted_message << "]\n";
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "\t\treceiveCDCD error" << std::endl;
                return false;
            }
        }
        std::cout << "Receive End\n";
        return false;
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
        openlog("Program G4 [CDCD] ", LOG_PID, LOG_LOCAL4);
        syslog(LOG_NOTICE, "%s", message.c_str());
        closelog();
    }
};

#endif
