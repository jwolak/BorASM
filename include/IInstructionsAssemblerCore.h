#pragma once

#include <string>
#include <vector>

namespace assembly_engine {
    class IInstructionsAssemblerCore {
      public:
        virtual ~IInstructionsAssemblerCore() = default;
        virtual void AssembleInstruction(const std::vector<std::string>& tokens) = 0;
    };
}  // namespace assembly_engine