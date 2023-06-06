#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fstream>
#include <filesystem>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

namespace fs = std::filesystem;

class FileManager {
 public:
    struct FoundFile
    {
        std::string directoryPath;
        std::string filePath;
    };

	std::vector<std::string> searchForMessages(){
		std::string directoryPath = "/home/fabian.gonzalezrojas/EAEA"; ///ruta/al/directorio
		std::string lastFileProcessed = "000000.txt"; // Reemplaza con el nombre del archivo que estás buscando

		fs::path directory(directoryPath);
		std::vector<FoundFile> foundFiles;
		std::vector<std::string> messages;

		if (fs::exists(directory) && fs::is_directory(directory))
		{
			searchFiles(directory, lastFileProcessed, foundFiles);

			if (!foundFiles.empty())
			{
				std::cout << "Archivos encontrados:" << std::endl;
				for (const auto& file : foundFiles)
				{
					std::cout << "Directorio: " << file.directoryPath << std::endl;
					std::cout << "Ruta relativa del archivo: " << file.filePath << std::endl;
					std::string nextFile = this->Read(file.filePath);
					std::string nextFilePath = file.directoryPath + "/" + nextFile + ".txt"; 
					std::cout << "Next file: "<< nextFilePath << std::endl; 
					std::string messageContent = this->Read(nextFilePath);
					if(messageContent != ""){
						std::cout << "Next file content: "<< messageContent << std::endl; 
						messages.push_back(messageContent);
					}
					std::cout << std::endl;
				}
			}
			else
			{
				std::cout << "No se encontraron archivos." << std::endl;
			}
		}
		else
		{
			std::cout << "El directorio especificado no existe." << std::endl;
		}
		return messages;
	}

	void saveMessage(std::string message){
		int result = -1;
		std::vector<std::string> messageParts = SplitMessageFile(message);
		std::string directoryPath = "/home/fabian.gonzalezrojas/EAEA/"+messageParts.at(0);
		std::string next_filename = directoryPath + "/next.txt";
		std::string filename = FileManager::Read(next_filename);
		int file_count = std::stoi(filename);
		if(file_count > -1 && file_count < 1000000){
			// Valid filename
			//std::cout << "Filename: " << filename << std::endl;
			//std::cout << "File count: " << file_count << std::endl;
			std::string filePath = directoryPath + "/" + filename + ".txt";
			//std::cout << "This path is about to be written: "<<filePath<<std::endl;
			FileManager::Write(message, filePath);
			file_count++;
			//std::cout << "File count: " << file_count << std::endl;
			filename = this->convertToZeroPaddedString(file_count);
			//std::cout << "Next filename: " << filename << std::endl;
			FileManager::Write(filename, next_filename);
			result = 0;
		} else {
			std::cout << "Ha ocurrido un error. No se pueden generar nuevos mensajes porque se ha excedido el limite.\n";
		}
		if(result == 0){
			//std::cout << "The message has been successfully stored. \n";
		} else if(result == -1){
			std::cout << "Error: Message wasn't saved. \n";
		}
	}

	std::vector<std::string> SplitMessageFile(const std::string& fileContent){
		std::vector<std::string> messageParts;
		if(!fileContent.empty()){
			std::vector<std::string> lines;
			std::istringstream iss(fileContent);
			std::string line;
			
			while (std::getline(iss, line)) {
				lines.push_back(line);
			}

			messageParts.push_back(lines.front());
			std::string message = lines.back();
			lines.pop_back();
			std::string hashed;
			for(int i = 1; i < lines.size(); i++){
				hashed+=lines.at(i);
			}
			messageParts.push_back(hashed);
			messageParts.push_back(message);
			// for(int i = 0; i < messageParts.size(); i++){
			// 	std::cout << "Part "<< i << ": "<<messageParts.at(i) << std::endl;
			// }
		}
		return messageParts;
	}

	std::string Read(const std::string& filename) {
		std::ifstream file(filename);
		if (!file) {
		std::cerr << "Error: Could not open file '" << filename << "' for reading." << std::endl;
		return "";
		}
		std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
		file.close();
		return content;
	}

	bool Write(const std::string& content, const std::string& filename) {
		std::ofstream file(filename);
		if (!file) {
		std::cerr << "Error: Could not open file '" << filename << "' for writing." << std::endl;
		return false;
		}
		file << content;
		file.close();
		return true;
	}

  private:
	
	void searchFiles(const fs::path& directory, const std::string& filename, std::vector<FoundFile>& foundFiles)
	{
		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (fs::is_directory(entry))
			{
				searchFiles(entry.path(), filename, foundFiles);  // Llamada recursiva para buscar en subdirectorios
			}
			else if (fs::is_regular_file(entry) && entry.path().filename() == filename)
			{
				FoundFile foundFile;
				foundFile.directoryPath = directory.string();
				foundFile.filePath = directory.string() + "/" + entry.path().filename().string();
				foundFiles.push_back(foundFile);
			}
		}
	}
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