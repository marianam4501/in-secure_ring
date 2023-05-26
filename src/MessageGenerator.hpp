#ifndef MESSAGE_GENERATOR_HPP
#define MESSAGE_GENERATOR_HPP

#include "FileManager.hpp"
#include <iostream>

class MessageGenerator {
 public:
  int createMessage(const std::string message) {
    std::string common_path = "/home/fabian.gonzalezrojas/CDCD/";
    std::string next_filename = common_path + "next.txt";
    std::string filename = FileManager::Read(next_filename);
    int file_count = std::stoi(filename);
    if(file_count > -1 && file_count < 1000000){
        // Valid filename
        std::cout << "Filename: " << filename << std::endl;
        std::cout << "File count: " << file_count << std::endl;
        std::cout << "This path is about to be written: "<<common_path + filename + ".txt"<<std::endl;
        FileManager::Write(message, common_path + filename + ".txt");
        file_count++;
        std::cout << "File count: " << file_count << std::endl;
        filename = this->convertToZeroPaddedString(file_count);
        std::cout << "Next filename: " << filename << std::endl;
        FileManager::Write(filename, next_filename);
        return 0;
    } else {
        std::cout << "Ha ocurrido un error. No se pueden generar nuevos mensajes porque se ha excedido el limite.\n";
        return -1;
    }
  }

  private:
    std::string convertToZeroPaddedString(int number)
    {
        std::string numberString = std::to_string(number);
        std::string zeroPaddedString = numberString;

        // Agregar ceros a la izquierda si es necesario
        while (zeroPaddedString.length() < 6)
        {
            zeroPaddedString = "0" + zeroPaddedString;
        }

        return zeroPaddedString;
    }
};

#endif