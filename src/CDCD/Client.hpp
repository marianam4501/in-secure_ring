#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

using namespace std;

class Client {
  private:
    std::string clientIP;

  public:
    Client(std::string clientIP) {
      this->clientIP = clientIP;
    }
  
    int send(const std::string& message)
    {
      int client_fd;
      struct sockaddr_in serv_addr;
      const char* messageP = message.c_str();
      char buffer[1024] = {0};
      if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      {
        std::cout << "\n Socket creation error \n";
        return -1;
      }
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(PORT);
      // Convert IPv4 and IPv6 addresses from text to binary form
      if (inet_pton(AF_INET, this->clientIP.c_str(), &serv_addr.sin_addr) <= 0)
      {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
      }
      if (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
      {
        std::cout << "\nConnection Failed \n";
        return -1;
      }
      size_t totalSent = 0;
      size_t messageSize = message.length();
      while (totalSent < messageSize) {
        ssize_t sent = ::send(client_fd, messageP + totalSent, messageSize - totalSent, 0);
        if (sent == -1)
        {
          std::cout << "\nError sending message\n";
          return -1;
        }
        totalSent += sent;
      }
      if (totalSent != messageSize) {
        std::cout << "\nError sending complete message\n";
        return -1;
      }
      ssize_t valread = read(client_fd, buffer, 1024);
      if (valread > 0) {
        std::cout << "Message sent from client\n";
      }
      std::cout << buffer << std::endl;
      // closing the connected socket
      close(client_fd);
      return 0;
    }
};
