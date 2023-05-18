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
    std::cout << "Input: [" << argv[1] << "] " << "[" << argv[2] << "]\n\n"; 
    /*
    if (strcmp(argv[1], "s") == 0) {
        std::cout << "Sender\n";
        EAEA channel(argv[2]);
    } else if (strcmp(argv[1], "m") == 0) {
        std::cout << "Middle\n";
        EAEA channel(argv[2]);
    } else if (strcmp(argv[1], "r") == 0) {
        EAEA channel();
        std::cout << "Receiver\n";
    }
    */
    return 0;
}