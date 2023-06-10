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

#define PORT 60138

class Server {
  private:
    int server_fd_;
    int new_socket_;
    struct sockaddr_in address_;
    std::string serverIP;

  public:
    Server(std::string serverIP);

    ~Server();

  private:
    void prepareServer();

  public:
    std::vector<unsigned char> start();
};

#endif