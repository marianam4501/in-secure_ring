#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h> // Para la comunicación en red
#include <cstdio> // Para entrada y salida estándar
#include <cstdlib> // Para funciones generales como malloc(), exit(), etc.
#include <cstring> // Para funciones de manipulación de cadenas
#include <sys/socket.h> // Para la creación y manipulación de sockets
#include <unistd.h> // Para funciones de sistema como sleep(), close(), etc.
#include <arpa/inet.h> // Para la manipulación de direcciones IP
#include <stdexcept> // Para excepciones estándar de C++
#include <iostream>

#define PORT 8080

class Client {
  private:
    int sock_;
    struct sockaddr_in server_;
    std::string clientIP;

  public:
    Client(std::string clientIP) {
        if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("socket failed");
        }
        memset(&server_, '0', sizeof(server_));
        server_.sin_family = AF_INET;
        server_.sin_port = htons(PORT);
        this->clientIP = clientIP;
    }

    ~Client() {
        close(sock_);
    }

    bool connect(const long port = 0) {
        if(port != 0) {
            server_.sin_port = htons(port);
        }
        if (inet_pton(AF_INET, this->clientIP.c_str(), &server_.sin_addr) <= 0) {
            // throw std::runtime_error("Invalid address/ Address not supported");
            return false;
        }
        if (::connect(sock_, (struct sockaddr*)&server_, sizeof(server_)) < 0) {
            // throw std::runtime_error("Connection Failed");
            return false;
        }
        return true;
    }

    void send(const char* message) {
        if (::send(sock_, message, strlen(message), 0) < 0) {
            throw std::runtime_error("Send failed");
        }
        char buffer[1024] = { 0 };
        int valread = read(sock_, buffer, 1024);
        // printf("%s\n", buffer);
    }
};

#endif