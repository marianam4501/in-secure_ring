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

void *ringComputerThread(void *arg)
{
    long id = (long)arg;
    printf("This is ringComputerThread [%ld]\n", (long)arg);
    if(id == 1) {
        RingComputerSender sender;
        sleep(2);
        sender.runChannels();
    }
    if(id == 2) {
        RingComputerMiddle middle;
        sleep(1);
        middle.runChannels();
    }
    if(id == 3) {
        RingComputerReceiver receiver;
        receiver.runChannels();
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t my_thread[3];
    long id;
    for(id = 1; id <= 3; id++) {
        int ret =  pthread_create(&my_thread[id], NULL, &ringComputerThread, (void*)id);
        if(ret != 0) {
            printf("Error: pthread_create() failed\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
    return 0;
}