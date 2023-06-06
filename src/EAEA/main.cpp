#include <iostream>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <chrono>

#include "EAEA.hpp"

void daemonize()
{
    pid_t pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    pid_t sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char *argv[]) {
    daemonize();
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