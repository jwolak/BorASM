#include "InstructionsAssemblerCore.h"

namespace assembly_engine {

    InstructionsAssemblerCore::InstructionsAssemblerCore(std::vector<uint8_t>& machineCode) : machineCode(machineCode) {
        // Constructor implementation
    }

    void InstructionsAssemblerCore::AssembleInstruction(const std::vector<std::string>& tokens) {}
}  // namespace assembly_engine
