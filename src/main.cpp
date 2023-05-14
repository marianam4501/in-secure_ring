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

int main(int argc, char* argv[]) {
    RingComputerSender prueba;
    prueba.runChannels();
    return 0;
}

/*
void *worker_thread(void *arg)
{
        printf("This is worker_thread #%ld\n", (long)arg);
        pthread_exit(NULL);
}

int main() {
    pthread_t my_thread[2];
    long id;
    for(id = 1; id <= 2; id++) {
        int ret =  pthread_create(&my_thread[id], NULL, &worker_thread, (void*)id);
        if(ret != 0) {
            printf("Error: pthread_create() failed\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
    return 0;
}
*/