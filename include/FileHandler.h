#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace assembly_engine {
    class FileHandler {
      public:
        FileHandler();
        ~FileHandler();
        FileHandler(const FileHandler&) = delete;
        FileHandler& operator=(const FileHandler&) = delete;
        FileHandler& operator=(FileHandler&& other) noexcept;
        bool OpenFileToRead(const std::string& file_path);
        bool OpenFileToWrite(const std::string& file_path);
        std::ifstream& GetFileToReadStream();
        std::ofstream& GetFileToWriteStream();

      private:
        std::ifstream file_to_read_;
        std::ofstream file_to_write_;
    };
}  // namespace assembly_engine