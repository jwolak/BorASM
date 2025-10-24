#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "CharacterStringLineHandler.h"
#include "IInstructionsAssemblerCore.h"

namespace assembly_engine {
    class InstructionsAssemblerCore : public IInstructionsAssemblerCore {
      public:
        InstructionsAssemblerCore(std::vector<uint8_t>& machine_code, std::vector<std::pair<int, std::string>>& label_references);
        bool AssembleInstruction(const std::vector<std::string>& tokens) override;

      private:
        std::vector<uint8_t>& machine_code_;
        std::vector<std::pair<int, std::string>>& label_references_;
        std::unique_ptr<CharacterStringLineHandler> character_string_line_handler_;
    };
}  // namespace assembly_engine