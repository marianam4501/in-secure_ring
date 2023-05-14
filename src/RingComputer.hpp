#ifndef RINGCOMPUTER_HPP
#define RINGCOMPUTER_HPP

#include <iostream>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "CDCD.hpp"
#include "EAEA.hpp"

#ifndef THREAD_COUNT
#define THREAD_COUNT 2
#endif


class RingComputer {
  protected:
    CDCD* cdcd;
    EAEA* eaea;

  public:
    RingComputer() {
      this->cdcd = new CDCD();
      this->eaea = new EAEA();
    }

    ~RingComputer() {
        free(cdcd);
        free(eaea);
    }

    void runChannels() {
      /* Override this method */
    }
};

#endif