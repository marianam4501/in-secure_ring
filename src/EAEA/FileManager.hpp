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
  private:
    const std::string PATH_USER = "fabian.gonzalezrojas";

  public:
    struct FoundFile
    {
        std::string directoryPath;
        std::string filePath;
    };

    FileManager();

    ~FileManager();

	  std::vector<std::string> searchForMessages(std::vector<std::string>& directories);

	  void saveMessage(std::string message);

	  std::vector<std::string> SplitMessageFile(const std::string& fileContent);

	  std::string Read(const std::string& filename);

	  bool Write(const std::string& content, const std::string& filename);

	  bool WriteAppend(const std::string& content, const std::string& filename);

	  std::string convertToZeroPaddedString(int number);

  private:
	  void searchFiles(const fs::path& directory, const std::string& filename, std::vector<FoundFile>& foundFiles);
};

#endif