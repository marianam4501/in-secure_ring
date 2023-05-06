#include <fstream>
#include <chrono>

#include <iostream>
#include "client.hpp"
#include "server.hpp"

/**
 * @return Error code, 0 on success
 */
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
