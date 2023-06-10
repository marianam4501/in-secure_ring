#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 52685

class Client {
  private:
    std::string sourceIP;
    std::string destinyIP;

  public:
    Client(std::string sourceIP, std::string destinyIP);

    ~Client();
  
    int send(const std::string& message);
};

#endif
