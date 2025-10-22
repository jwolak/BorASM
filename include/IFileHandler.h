#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace assembly_engine {
    class IFileHandler {
      public:
        virtual ~IFileHandler() = default;
        virtual bool OpenFileToRead(const std::string& file_path) = 0;
        virtual bool OpenFileToWrite(const std::string& file_path) = 0;
        virtual std::ifstream& GetFileToReadStream() = 0;
        virtual std::ofstream& GetFileToWriteStream() = 0;
    };
}  // namespace assembly_engine