#include <iostream>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <fstream>
#include <chrono>

#include "EAEA.hpp"

int main(int argc, char *argv[]) {
    std::string type = argv[1];
    std::string serverIP = argv[2];
    std::string clientIP = argv[3];
    std::cout << "Input: [" << type << "] " << "[" << serverIP << "]" << "[" << clientIP << "]\n\n";
    EAEA channel(type, serverIP, clientIP);
    const bool stop = false;
    while (!stop)
        if (!channel.run()) {
            std::cout << "Runtime error: relaunching\n" ;
            channel.errorLog(); // Might be wise checking the integrity of the program in this routine
        }
    return 0;
}