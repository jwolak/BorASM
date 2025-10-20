#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace assembly_engine {
    class InstructionsAssemblerCore {
      public:
        InstructionsAssemblerCore(std::vector<uint8_t>& machineCode);
        void AssembleInstruction(const std::vector<std::string>& tokens);

      private:
        std::vector<uint8_t>& machineCode;
    };
}  // namespace assembly_engine