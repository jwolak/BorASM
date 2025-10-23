#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <string>

#include "CharacterStringLineHandler.h"
#include "InstructionsAssemblerCore.h"

namespace assembly_engine {
    class ICodeAnalyzer {
      public:
        virtual ~ICodeAnalyzer() = default;
        virtual bool DetectLabels(std::ifstream& file, std::string& line) = 0;
        virtual bool Tokenize(std::ifstream& file, std::string& line) = 0;
        virtual bool ResolveLabelReferences() = 0;
    };
}  // namespace assembly_engine