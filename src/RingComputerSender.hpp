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

    void static *worker_thread(void *arg)
    {
        thread_info_t thread_info = (thread_info_t)arg;
        printf("This is worker_thread #%ld\n", thread_info.id);
        if(thread_info.id == 1) {
            thread_info.myComputer->eaea->receiveEAEA("127.0.0.1");
        }
        if(thread_info.id == 2) {
            thread_info.myComputer->eaea->sendEAEA("127.0.0.1");
        }
        pthread_exit(NULL);
    }

    void runChannels() {
        // run eaea
        // run cdcd
        pthread_t my_thread[2];
        thread_info_t thread_info;
        thread_info.myComputer = this;
        for(thread_info.id = 1; thread_info.id <= 2; thread_info.id++) {
            int ret =  pthread_create(&my_thread[thread_info.id], NULL, &RingComputerSender::worker_thread, (void*)thread_info.id);
            if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
            }
        }
        pthread_exit(NULL);
        
        /*
        #pragma omp parallel for num_threads(omp_get_thread_num()) default(none) schedule(static)
        if(omp_get_thread_num() == 0) {
            printf("Canal EAEA");
        } else {
            printf("Canal CDCD");
        }
        */
    }
};

#endif