#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <vector>

#define PORT 8080

class Server {
  private:
    int server_fd_;
    int new_socket_;
    struct sockaddr_in address_;
    std::string serverIP;

  public:
    Server(std::string serverIP, const long port = 0) {
        this->serverIP = serverIP;
        // Creating socket file descriptor
        if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("socket failed");
        }

        int opt = 1;
        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            throw std::runtime_error("setsockopt");
        }

        address_.sin_family = AF_INET;
        address_.sin_addr.s_addr = inet_addr(this->serverIP.c_str());
        if(port == 0) {
            address_.sin_port = htons(PORT);
        } else {
            address_.sin_port = port;
        }

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd_, (struct sockaddr*)&address_, sizeof(address_)) < 0) {
            throw std::runtime_error("bind failed");
        }
    }

    ~Server() {
        close(new_socket_);
        close(server_fd_);
    }

    std::vector<unsigned char> start() {
        if (listen(server_fd_, 3) < 0) {
            throw std::runtime_error("listen");
        }
        int addrlen = sizeof(address_);
        if ((new_socket_ = accept(server_fd_, (struct sockaddr*)&address_, (socklen_t*)&addrlen)) < 0) {
            throw std::runtime_error("accept");
        }
        char* buffer = new char[1024];
        const char* hello = "\t(Respuesta del servidor)";
        size_t totalReceived = 0;
        while (totalReceived < sizeof(buffer)) {
            ssize_t bytesReceived = read(new_socket_, buffer + totalReceived, sizeof(buffer) - totalReceived);
            if (bytesReceived < 0) {
                throw std::runtime_error("Read failed");
            }
            totalReceived += bytesReceived;
        }
        if (totalReceived != sizeof(buffer)) {
            throw std::runtime_error("Not all bytes were received");
        }
        const std::vector<unsigned char> received_message(buffer, buffer + totalReceived);
        for (const auto& element : received_message) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        send(new_socket_, hello, strlen(hello), 0);
        return received_message;
    }
};

#endif