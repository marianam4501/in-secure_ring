#ifndef CDCD_HPP
#define CDCD_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "Server.hpp"
#include "Client.hpp"
#include "Cryptographer.hpp"
#include <vector>

class CDCD {
  private:
    Server *server;
    Client *client;
    Cryptographer *cryptographer;
    std::string type;
    std::string clientIP;

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
        while (!stop) {
            try {
                // TODO: get message from Filemanager (also validate this message)
                std::string message = "Intento No." + std::to_string(sended);
                message = this->cryptographer->encrypt(message,"./src/public_key.pem"); 
                
                this->client->send(message,clientIP);
                std::cout << "Sended: [" << message << "]\n";
                std::cout << "Length: [" << message.length() << "]\n";
                // TODO: Log this information
                
                ++sended;
                if(sended == 2) {
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
                std::string decrypted_message = this->cryptographer->decrypt(received_message.c_str(),"./src/private_key.pem");
                // TODO: Log this information
                // TODO: Store this information
                std::cout << "Received: [" << decrypted_message << "]\n";
                //std::cout << "Length: [" << message.length() << "]\n";
                //sleep(4);
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
};

#endif
