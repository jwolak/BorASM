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
        bool OpenFile(const std::string& file_path, std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary);
        bool OpenFileToWrite(const std::string& file_path);
        std::ifstream& GetFileStream();
        std::ofstream& GetFileToWriteStream();

      private:
        std::ifstream file_to_read_;
        std::ofstream file_to_write_;
    };
}  // namespace assembly_engine