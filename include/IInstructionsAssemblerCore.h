#pragma once

#include <string>
#include <vector>

namespace assembly_engine {
    class IInstructionsAssemblerCore {
      public:
        virtual ~IInstructionsAssemblerCore() = default;
        virtual bool AssembleInstruction(const std::vector<std::string>& tokens) = 0;
    };
}  // namespace assembly_engine