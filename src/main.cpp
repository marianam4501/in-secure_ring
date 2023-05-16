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

#include "RingComputerSender.hpp"
#include "RingComputerMiddle.hpp"
#include "RingComputerReceiver.hpp"

int main(int argc, char *argv[]) {
    std::cout << "Input: [" << argv[1] << "] " << "[" << argv[2] << "] " << "[" << argv[3] << "]\n\n"; 
    bool cdcdUp = false;
    bool eaeaUp = false;
    if (strcmp(argv[2], "CDCD") == 0) { cdcdUp = true; }
    if (strcmp(argv[2], "EAEA") == 0) { eaeaUp = true; }
    if (strcmp(argv[1], "s") == 0) {
        std::cout << "Sender\n";
        RingComputerSender computer(cdcdUp, eaeaUp, argv[3]);
    } else if (strcmp(argv[1], "m") == 0) {
        std::cout << "Middle\n";
        RingComputerMiddle computer(cdcdUp, eaeaUp, argv[3]);
    } else if (strcmp(argv[1], "r") == 0) {
        RingComputerReceiver computer(cdcdUp, eaeaUp);
        std::cout << "Receiver\n";
    }
    return 0;
}