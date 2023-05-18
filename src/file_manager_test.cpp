#include <fstream>
#include <string>
#include <chrono>
#include <iostream>

#include "file_manager.hpp"

/**
 * @return Error code, 0 on success
 */
int main() {
    FileManager::Write("Hello world.", "test.txt");
    std::cout << FileManager::Read("test.txt") << std::endl;
    return 0;
}