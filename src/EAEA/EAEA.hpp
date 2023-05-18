#ifndef EAEA_HPP
#define EAEA_HPP

#include <iostream>
#include <string>

#include "Server.hpp"
#include "Client.hpp"

class EAEA {
  private:
    Server *server;
    Client *client;
    char* type;

  public:
    EAEA(char* type, const char* ipDirection) {
        // Sender
        if (strcmp(type, "s") == 0) {
            this->client = new Client(ipDirection);
            this->server = NULL;
        }
        if (strcmp(type, "m") == 0) {
            this->client = new Client(ipDirection);
            this->server = new Server();
        }
        if (strcmp(type, "r") == 0) {
            this->client = NULL;
            this->server = new Server();
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

    void run() {
        if (strcmp(this->type, "s") == 0) {
            this->sendEAEA();
        }
        if (strcmp(this->type, "m") == 0) {
            this->resendEAEA();
        }
        if (strcmp(this->type, "r") == 0) {
            this->receiveEAEA();
        }
    }

    void sendEAEA() {
        std::cout << "Send start\n";
        bool stop = false;
        unsigned int sended= 0;
        while (!stop) {
            try {
                // TODO: get message from Filemanager (also validate this message)
                char* message = "EAEA message 1 from sender";
                // TODO: attach certificate to socket before sending it
                if(this->client->connect()) {
                    this->client->send(message);
                    std::cout << "Sended: [" << message << "]\n";
                    // TODO: Log this information
                } else {
                    std::cout << "Connection failed" << std::endl;
                }
                sleep(4);
                ++sended;
                if(sended == 10) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "sendEAEA error" << std::endl;
            }
        }
        std::cout << "Send End\n";
    }

    void resendEAEA() {
        std::cout << "Resend start\n";
        bool stop = false;
        unsigned int sended= 0;
        while (!stop) {
            try {
                char* message = this->server->start();
                this->client->connect();
                this->client->send(message);
                std::cout << "Resended: [" << message << "]\n";
                // TODO: Log this information
                sleep(4);
                ++sended;
                if(sended == 10) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "resendEAEA error" << std::endl;
            }
        }
        std::cout << "Resend End\n";
    }

    void receiveEAEA() {
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
                sleep(4);
                ++received;
                if(received == 10) {
                    stop = true;
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "receiveEAEA error" << std::endl;
            }
        }
        std::cout << "Receive End\n";
    }
};

#endif