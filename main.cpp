#include <fstream>
#include <chrono>

#include "client.hpp"
#include "server.hpp"

/**
 * @return Error code, 0 on success
 */
int main() {
    try {
        Server server;
        server.start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}