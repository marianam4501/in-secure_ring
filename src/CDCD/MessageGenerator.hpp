#ifndef MESSAGE_GENERATOR_HPP
#define MESSAGE_GENERATOR_HPP

#include "FileManager.hpp"
#include <iostream>

class MessageGenerator {
 public:
    int createMessage(const std::string message);

    std::string convertToZeroPaddedString(int number);
};

#endif