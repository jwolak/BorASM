#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "CharacterStringLineHandler.h"
#include "FileHandler.h"

namespace assembly_engine {
    class AssemblyEngine {
      public:
        AssemblyEngine();
        bool Assemble(const std::string& input_file, const std::string& output_file);

      private:
        std::unique_ptr<FileHandler> file_handler_;
        std::unique_ptr<CharacterStringLineHandler> line_handler_;
        std::vector<uint8_t> machineCode;
        std::map<std::string, uint16_t> labels;
        std::vector<std::pair<int, std::string>> labelReferences;
    };
}  // namespace assembly_engine