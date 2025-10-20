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
        std::ifstream& GetFileStream();

      private:
        std::ifstream file;
    };
}  // namespace assembly_engine