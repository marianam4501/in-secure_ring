#include <iostream>
#include "MessageGenerator.hpp"

int main(){
    std::string message = "";
    std::cout << "Enter your message: ";
    std::getline(std::cin, message);
    std::cout << std::endl;
    MessageGenerator generator;
    generator.createMessage(message);
    return 0;
}