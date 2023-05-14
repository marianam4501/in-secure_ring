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

#define PORT 8080

class Server {
  private:
    int server_fd_;
    int new_socket_;
    struct sockaddr_in address_;
  public:
    Server() {
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
        address_.sin_addr.s_addr = INADDR_ANY;
        address_.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd_, (struct sockaddr*)&address_, sizeof(address_)) < 0) {
            throw std::runtime_error("bind failed");
        }
    }

    ~Server() {
        close(new_socket_);
        close(server_fd_);
    }

    char* start() {
        if (listen(server_fd_, 3) < 0) {
            throw std::runtime_error("listen");
        }
        int addrlen = sizeof(address_);
        if ((new_socket_ = accept(server_fd_, (struct sockaddr*)&address_, (socklen_t*)&addrlen)) < 0) {
            throw std::runtime_error("accept");
        }
        char* buffer = new char[1024];
        const char* hello = " (Respuesta del servidor)";
        int valread = read(new_socket_, buffer, 1024);
        printf("%s\n", buffer);
        send(new_socket_, hello, strlen(hello), 0);
        return buffer;
    }
};

#endif