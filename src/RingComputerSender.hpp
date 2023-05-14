#ifndef RINGCOMPUTERSENDER_HPP
#define RINGCOMPUTERSENDER_HPP

#include <iostream>

#include "CDCD.hpp"
#include "EAEA.hpp"
#include "RingComputer.hpp"



class RingComputerSender : public RingComputer {
  public:
    struct thread_info_t {
        long id;
        RingComputerSender* myComputer;
    };

  private:
    /* Add here message generator */

  public:
    RingComputerSender() {
    }

    ~RingComputerSender() {
    }

    void static *runEAEAChannel(void *arg)
    {
        EAEA eaea;
        long id = (long)arg;
        printf("EAEA_thread [%ld]\n", id);
        if(id == 1) {
            eaea.receiveEAEA("127.0.0.1");
        }
        if(id == 2) {
            eaea.sendEAEA("127.0.0.1", "Mensaje enviado desde canal EAEA");
        }
        pthread_exit(NULL);
    }

    void runChannels() {
        // run eaea
        // run cdcd
        pthread_t my_thread[2];
        long id = 0;
        for(id = 1; id <= 2; id++) {
            int ret =  pthread_create(&my_thread[id], NULL, &RingComputerSender::runEAEAChannel, (void*)id);
            if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
            }
        }
        pthread_exit(NULL);
    }
};

#endif