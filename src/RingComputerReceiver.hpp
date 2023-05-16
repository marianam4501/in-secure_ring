#ifndef RINGCOMPUTERRECEIVER_HPP
#define RINGCOMPUTERRECEIVER_HPP

#include <iostream>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "EAEA.hpp"
#include "CDCD.hpp"

class RingComputerReceiver {
  private:
    CDCD* cdcd;
    EAEA* eaea;

  public:
    RingComputerReceiver(const bool cdcdUp, const bool eaeaUp) {
        if (cdcdUp) {
            this->cdcd = new CDCD(true, false);
        } else {
            this->cdcd = NULL;
        }
        if (eaeaUp) {
            this->eaea = new EAEA(true, false);
        } else {
            this->eaea = NULL;
        }
    }

    ~RingComputerReceiver() {
        if (this->cdcd != NULL) {
            free(this->cdcd);
        }
        if (this->eaea != NULL) {
            free(this->eaea);
        }
    }

    void static *runEAEAChannel(void *arg)
    {
        /*
        EAEA eaea;
        long id = (long)arg;
        // printf("\tReceiver_thread [%ld]\n", id);
        if(id == 1) {
            //eaea.receiveEAEA("127.0.0.1");
            //eaea.sendEAEA("127.0.0.1", "Mensaje enviado desde canal EAEA");
        }
        if(id == 2) {
            printf("\tReceiver_thread [%ld]\n", id);
            eaea.receiveEAEA("127.0.0.1");
        }
        */
        pthread_exit(NULL);
    }

    void runChannels() {
        // run eaea
        // run cdcd
        pthread_t my_thread[2];
        long id = 0;
        for(id = 1; id <= 2; id++) {
            int ret =  pthread_create(&my_thread[id], NULL, &RingComputerReceiver::runEAEAChannel, (void*)id);
            if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
            }
        }
        pthread_exit(NULL);
    }
};

#endif