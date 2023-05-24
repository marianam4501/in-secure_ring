#ifndef CDCD_HPP
#define CDCD_HPP

#include <iostream>
#include <string>

#include "Server.hpp"
#include "Client.hpp"
#include "Cryptographer.hpp"

class CDCD {
  private:
    Server *server;
    Client *client;
    Cryptographer *crytographer;
    std::string type;
    std::string clientIP;

  public:
    CDCD(std::string type, std::string serverIP, std::string clientIP) {
        // Sender
        if (type.compare("s") == 0) {
            this->client = new Client(clientIP);
            this->server = NULL;
            this->crytographer = new Cryptographer();
        }
        if (type.compare("m") == 0) {
            this->client = new Client(clientIP);
            this->server = new Server(serverIP);
            this->crytographer = NULL;
        }
        if (type.compare("r") == 0) {
            this->client = NULL;
            this->server = new Server(serverIP);
            this->crytographer = new Cryptographer();
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
        if (this->crytographer != NULL) {
            free(this->crytographer);
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
                std::string message = "CDCD message " + std::to_string(sended) + " from sender";
                // TODO: encrypt message (also validate that encryption was done correctly)
                if (this->client->connect() != true) {
                    std::cout << "Connection failed" << std::endl;
                } else {
                    this->client->send(message.c_str());
                    std::cout << "Sended: [" << message << "]\n";
                }
                // TODO: Log this information
                free(this->client);
                this->client = new Client(this->clientIP);
                //sleep(3);
                ++sended;
                if(sended == 10) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "sendCDCD error" << std::endl;
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
                char* message = this->server->start();
                if (this->client->connect() != true) {
                    std::cout << "Connection failed" << std::endl;
                } else {
                    this->client->send(message);
                    std::cout << "Resend: [" << message << "]\n";
                }
                // TODO: Log this information
                //sleep(4);
                ++sended;
                free(this->client);
                this->client = new Client(this->clientIP);
                if(sended == 10) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "resendCDCD error" << std::endl;
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
                char* message = this->server->start();
                // TODO: Decrypt this information
                // TODO: Log this information
                // TODO: Store this information
                std::cout << "Received: [" << message << "]\n";
                //sleep(4);
                ++received;
                if(received == 10) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "receiveCDCD error" << std::endl;
            }
        }
        std::cout << "Receive End\n";
    }
};

#endif