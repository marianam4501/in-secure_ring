#include "Client.hpp"

Client::Client(std::string sourceIP, std::string destinyIP) {
  this->sourceIP = sourceIP;
  this->destinyIP = destinyIP;
}

Client::~Client() {}

int Client::send(const std::string& message)
{
  int client_fd;
  struct sockaddr_in serv_addr;
  struct sockaddr_in clientAddress;
  const char* messageP = message.c_str();
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "\n Socket creation error \n";
    return -1;
  }
  // Seteando la ip de donde voy a mandar el mensaje
  memset(&clientAddress, 0, sizeof(clientAddress));
  clientAddress.sin_family = AF_INET;
  clientAddress.sin_port = htons(0);
  if (inet_pton(AF_INET, this->sourceIP.c_str(), &clientAddress.sin_addr) <= 0) {
    std::cout << "\nInvalid address/ Address not supported \n";
    return -1;
  }
  // Seteando la ip a la que le voy a mandar el mensaje
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, this->destinyIP.c_str(), &serv_addr.sin_addr) <= 0) {
    std::cout << "\nInvalid address/ Address not supported \n";
    return -1;
  }
  if (bind(client_fd, (struct sockaddr *)&clientAddress, sizeof(clientAddress)) < 0) {
    std::cout << "Bind failed\n";
    return -1;
  }
  if (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
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
  char buffer[1024] = {0};
  ssize_t valread = read(client_fd, buffer, 1024);
  if (valread > 0) {
    std::cout << "Message sent from client\n";
  }
  std::cout << buffer << std::endl;
  close(client_fd);
  return 0;
}
