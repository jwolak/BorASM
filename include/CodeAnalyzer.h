#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <string>

#include "CharacterStringLineHandler.h"
#include "ICodeAnalyzer.h"
#include "InstructionsAssemblerCore.h"

namespace assembly_engine {
    class CodeAnalyzer : public ICodeAnalyzer {
      public:
        CodeAnalyzer(std::vector<uint8_t>& machine_code, std::map<std::string, uint16_t>& labels, std::vector<std::pair<int, std::string>>& label_references);
        bool DetectLabels(std::ifstream& file, std::string& line) override;
        bool Tokenize(std::ifstream& file, std::string& line) override;
        bool ResolveLabelReferences() override;

      protected:
        CodeAnalyzer(std::vector<uint8_t>& machine_code, std::map<std::string, uint16_t>& labels, std::vector<std::pair<int, std::string>>& label_references,
                     std::unique_ptr<ICharacterStringLineHandler> line_handle, std::unique_ptr<IInstructionsAssemblerCore> instructions_assembler_core);

      private:
        std::vector<uint8_t>& machine_code_;
        std::map<std::string, uint16_t>& labels_;
        std::vector<std::pair<int, std::string>>& label_references_;
        std::unique_ptr<ICharacterStringLineHandler> line_handler_;
        std::unique_ptr<IInstructionsAssemblerCore> instructions_assembler_core_;
    };
}  // namespace assembly_engine