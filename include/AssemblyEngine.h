#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "CharacterStringLineHandler.h"
#include "FileHandler.h"
#include "IAssemblyEngine.h"
#include "InstructionsAssemblerCore.h"
#include "LabelsDetector.h"

namespace assembly_engine {
    class AssemblyEngine : public IAssemblyEngine {
      public:
        AssemblyEngine();
        bool Assemble(const std::string& input_file, const std::string& output_file) override;
        void PrintMachineCode() const override;
        bool SaveMachineCodeToFile(const std::string& output_file) const override;

      protected:
        AssemblyEngine(std::unique_ptr<IFileHandler> file_handler, std::unique_ptr<ICharacterStringLineHandler> line_handler,
                       std::unique_ptr<IInstructionsAssemblerCore> instructions_assembler_core);

      private:
        std::unique_ptr<IFileHandler> file_handler_;
        std::unique_ptr<ICharacterStringLineHandler> line_handler_;
        std::vector<uint8_t> machine_code_;
        std::map<std::string, uint16_t> labels_;
        std::vector<std::pair<int, std::string>> label_references_;
        std::unique_ptr<IInstructionsAssemblerCore> instructions_assembler_core_;
        std::unique_ptr<CodeAnalyzer> code_analyzer_;
    };
}  // namespace assembly_engine