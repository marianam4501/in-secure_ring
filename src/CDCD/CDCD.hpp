#ifndef CDCD_HPP
#define CDCD_HPP

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
  private:
    Server *server;
    Client *client;
    Cryptographer *cryptographer;
    std::string type;
    std::string clientIP;
    MessageGenerator generator;

  public:
    CDCD(std::string type, std::string serverIP, std::string clientIP) {
        // Sender
        if (type.compare("s") == 0) {
            this->client = new Client();
            this->server = NULL;
            this->cryptographer = new Cryptographer();
        }
        if (type.compare("m") == 0) {
            this->client = new Client();
            this->server = new Server(serverIP);
            this->cryptographer = NULL;
        }
        if (type.compare("r") == 0) {
            this->client = NULL;
            this->server = new Server(serverIP);
            this->cryptographer = new Cryptographer();
        }
        this->type = type;
        this->clientIP = clientIP;
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

    void run() {
        if (type.compare("s") == 0) {
            this->send();
        }
        if (type.compare("m") == 0) {
            this->resend();
        }
        if (type.compare("r") == 0) {
            this->receive();
        }
    }

  private:
    void send() {
        std::cout << "Send start\n";
        bool stop = false;
        unsigned int sended= 0;
        std::string message_count_path = "/home/mariana.murilloquintana/CDCD/000000.txt";
        std::string path = "/home/mariana.murilloquintana/CDCD/";
        std::string last_msg_processed = FileManager::Read(message_count_path);
        while (!stop) {
            try {
                // TODO: get message from Filemanager (also validate this message)
                std::string message = "";
                if(last_msg_processed != "000000"){
                    message = FileManager::Read(path+last_msg_processed+".txt");
                    if(!message.empty()){
                        std::cout<<"Sending "<<last_msg_processed<<".txt..."<<std::endl;
                        message = this->cryptographer->encrypt(message,"./src/public_key.pem"); 
                        this->client->send(message,clientIP);
                        std::cout << "Sended: [" << message << "]\n";
                        std::cout << "Length: [" << message.length() << "]\n";
                        int file_count = std::stoi(last_msg_processed);
                        file_count++;
                        last_msg_processed = convertToZeroPaddedString(file_count);
                        FileManager::Write(last_msg_processed, message_count_path);
                        ++sended;
                    } else {
                        //std::cout << "File doesnt exist. " << std::endl;
                    }
                } else {
                    std::cout << "There is nothing to process yet." << std::endl;
                }
                // TODO: Log this information
                
                if(sended == 20) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "\t\tsendCDCD error" << std::endl;
            }
        }
        std::cout << "Send End\n";
    }

    void resend() {
        std::cout << "Resend start\n";
        bool stop = false;
        unsigned int sended= 0;
        while (!stop) {
            try {
                std::vector<unsigned char> message = this->server->start();
                std::string received_message(message.begin(), message.end());
                this->client->send(received_message,this->clientIP);
                std::cout << "Resend: [" << received_message << "]\n";
                // TODO: Log this information
                ++sended;
                if(sended == 2) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "\t\tresendCDCD error" << std::endl;
            }
        }
        std::cout << "Resend End\n";
    }

    void receive() {
        std::cout << "Receive start\n";
        bool stop = false;
        unsigned int received= 0;
        while (!stop) {
            try {
                std::vector<unsigned char> message = this->server->start();
                std::string received_message(message.begin(), message.end());
                std::cout << "Length received: [" << received_message.length() << "]\n";
                std::string decrypted_message = this->cryptographer->decrypt(message,"./src/private_key.pem");
                // TODO: Log this information
                // TODO: Store this information
                generator.createMessage(decrypted_message);
                std::cout << "Received: [" << decrypted_message << "]\n";
                ++received;
                if(received == 100) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "\t\treceiveCDCD error" << std::endl;
            }
        }
        std::cout << "Receive End\n";
    }

    private:
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
};

#endif
