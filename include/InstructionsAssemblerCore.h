#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "CharacterStringLineHandler.h"

namespace assembly_engine {
    class InstructionsAssemblerCore {
      public:
        InstructionsAssemblerCore(std::vector<uint8_t>& machineCode, std::vector<std::pair<int, std::string>>& labelReferences);
        void AssembleInstruction(const std::vector<std::string>& tokens);

      private:
        std::vector<uint8_t>& machineCode;
        std::vector<std::pair<int, std::string>>& labelReferences;
        std::unique_ptr<CharacterStringLineHandler> character_string_line_handler_;
    };
}  // namespace assembly_engine