#pragma once

#include <memory>
#include <string>

#include "FileHandler.h"

namespace assembly_engine {
    class AssemblyEngine {
      public:
        AssemblyEngine();
        bool Assemble(const std::string& input_file, const std::string& output_file);

      private:
        std::unique_ptr<FileHandler> file_handler_;
    };
}  // namespace assembly_engine