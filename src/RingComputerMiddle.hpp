#ifndef RINGCOMPUTERMIDDLE_HPP
#define RINGCOMPUTERMIDDLE_HPP

#include <iostream>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "RingComputer.hpp"

class RingComputerMiddle : public RingComputer {
  public:
    struct thread_info_t {
        long id;
        RingComputerMiddle* myComputer;
    };

  private:
    /* Add here message generator */

  public:
    RingComputerMiddle() {
    }

    ~RingComputerMiddle() {
    }

    void static *runEAEAChannel(void *arg)
    {
        EAEA eaea;
        long id = (long)arg;
        //printf("\tMiddle_thread [%ld]\n", id);
        if(id == 1) {
            //eaea.receiveEAEA("127.0.0.1");
            //eaea.sendEAEA("127.0.0.1", "Mensaje enviado desde canal EAEA");
        }
        if(id == 2) {
            printf("\tMiddle_thread [%ld]\n", id);
            eaea.receiveSendEAEA("127.0.0.1", 8081);
        }
        pthread_exit(NULL);
    }

    void runChannels() {
        // run eaea
        // run cdcd
        pthread_t my_thread[2];
        long id = 0;
        for(id = 1; id <= 2; id++) {
            int ret =  pthread_create(&my_thread[id], NULL, &RingComputerMiddle::runEAEAChannel, (void*)id);
            if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
            }
        }
        pthread_exit(NULL);
    }
};

#endif