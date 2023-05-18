#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fstream>
#include <iostream>
#include <string>

class FileManager {
 public:
  static std::string Read(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
      std::cerr << "Error: Could not open file '" << filename << "' for reading." << std::endl;
      return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    file.close();
    return content;
  }

  static bool Write(const std::string& content, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
      std::cerr << "Error: Could not open file '" << filename << "' for writing." << std::endl;
      return false;
    }
    file << content;
    file.close();
    return true;
  }
};

#endif