#include <netinet/in.h> // Para la comunicación en red
#include <cstdio> // Para entrada y salida estándar
#include <cstdlib> // Para funciones generales como malloc(), exit(), etc.
#include <cstring> // Para funciones de manipulación de cadenas
#include <sys/socket.h> // Para la creación y manipulación de sockets
#include <unistd.h> // Para funciones de sistema como sleep(), close(), etc.
#include <arpa/inet.h> // Para la manipulación de direcciones IP
#include <stdexcept> // Para excepciones estándar de C++


#define PORT 8080

class Client {
public:
    Client();
    ~Client();
    void connect(const char* server_address);
    void send(const char* message);
private:
    int sock_;
    struct sockaddr_in server_;
};

Client::Client() {
    if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error("socket failed");
    }
    memset(&server_, '0', sizeof(server_));
    server_.sin_family = AF_INET;
    server_.sin_port = htons(PORT);
}

Client::~Client() {
    close(sock_);
}

void Client::connect(const char* server_address) {
    if (inet_pton(AF_INET, server_address, &server_.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address/ Address not supported");
    }
    if (::connect(sock_, (struct sockaddr*)&server_, sizeof(server_)) < 0) {
        throw std::runtime_error("Connection Failed");
    }
}

void Client::send(const char* message) {
    if (::send(sock_, message, strlen(message), 0) < 0) {
        throw std::runtime_error("Send failed");
    }
    char buffer[1024] = { 0 };
    int valread = read(sock_, buffer, 1024);
    printf("%s\n", buffer);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <server_address>\n", argv[0]);
        return 1;
    }
    try {
        Client client;
        client.connect(argv[1]);
        client.send("Hello from client");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
