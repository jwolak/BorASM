#include "LabelsDetector.h"

#include <fstream>
#include <map>
#include <memory>
#include <string>

#include "CharacterStringLineHandler.h"
#include "InstructionsAssemblerCore.h"

namespace assembly_engine {
    LabelsDetector::LabelsDetector(std::vector<uint8_t>& machine_code, std::map<std::string, uint16_t>& labels,
                                   std::vector<std::pair<int, std::string>>& label_references)
        : machine_code_(machine_code),
          labels_(labels),
          label_references_(label_references),
          line_handler_(std::make_unique<CharacterStringLineHandler>()),
          instructions_assembler_core_(std::make_unique<InstructionsAssemblerCore>(machine_code_, label_references_)) {}

    LabelsDetector::LabelsDetector(std::vector<uint8_t>& machine_code, std::map<std::string, uint16_t>& labels,
                                   std::vector<std::pair<int, std::string>>& label_references, std::unique_ptr<ICharacterStringLineHandler> line_handle,
                                   std::unique_ptr<IInstructionsAssemblerCore> instructions_assembler_core)

        : machine_code_(machine_code),
          labels_(labels),
          label_references_(label_references),
          line_handler_(std::move(line_handle)),
          instructions_assembler_core_(std::move(instructions_assembler_core)) {}

    bool LabelsDetector::DetectLabels(std::ifstream& file, const std::string& line) {
        // Implement label detection logic here
        return false;
    }

}  // namespace assembly_engine