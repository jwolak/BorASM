#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "IFileHandler.h"

namespace assembly_engine {
    class FileHandler : public IFileHandler {
      public:
        FileHandler();
        ~FileHandler();
        FileHandler(const FileHandler&) = delete;
        FileHandler& operator=(const FileHandler&) = delete;
        FileHandler& operator=(FileHandler&& other) noexcept;
        bool OpenFileToRead(const std::string& file_path) override;
        bool OpenFileToWrite(const std::string& file_path) override;
        std::ifstream& GetFileToReadStream() override;
        std::ofstream& GetFileToWriteStream() override;

      private:
        std::ifstream file_to_read_;
        std::ofstream file_to_write_;
    };
}  // namespace assembly_engine