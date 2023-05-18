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

#include "CDCD.hpp"

int main(int argc, char *argv[]) {
    std::cout << "Input: [" << argv[1] << "] " << "[" << argv[2] << "]\n\n"; 
    CDCD channel(argv[1], argv[2]);
    channel.run();
    return 0;
}