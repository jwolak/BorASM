#include "CodeAnalyzer.h"

#include <fstream>
#include <map>
#include <memory>
#include <string>

#include "CharacterStringLineHandler.h"
#include "InstructionsAssemblerCore.h"
#include "Tools.h"
#include "EquinoxLogger.hpp"

namespace assembly_engine {
    CodeAnalyzer::CodeAnalyzer(std::vector<uint8_t>& machine_code, std::map<std::string, uint16_t>& labels,
                               std::vector<std::pair<int, std::string>>& label_references)
        : machine_code_(machine_code),
          labels_(labels),
          label_references_(label_references),
          line_handler_(std::make_unique<CharacterStringLineHandler>()),
          instructions_assembler_core_(std::make_unique<InstructionsAssemblerCore>(machine_code_, label_references_)) {}

    /* For tests purposes */
    CodeAnalyzer::CodeAnalyzer(std::vector<uint8_t>& machine_code, std::map<std::string, uint16_t>& labels,
                               std::vector<std::pair<int, std::string>>& label_references, std::unique_ptr<ICharacterStringLineHandler> line_handle,
                               std::unique_ptr<IInstructionsAssemblerCore> instructions_assembler_core)
        : machine_code_(machine_code),
          labels_(labels),
          label_references_(label_references),
          line_handler_(std::move(line_handle)),
          instructions_assembler_core_(std::move(instructions_assembler_core)) {}

    bool CodeAnalyzer::DetectLabels(std::ifstream& file, std::string& line) {
        int lineNumber = 0;
        while (std::getline(file, line)) {
            ++lineNumber;
            LOG_DEBUG("[CodeAnalyzer] Processing line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            line = line_handler_->CleanLineWhitespaces(line);
            tools::PrintGreenOKMessage("Cleaned line " + std::to_string(lineNumber) + ": " + line);
            line = line_handler_->RemoveLineComments(line);
            tools::PrintGreenOKMessage("Removed comments from line " + std::to_string(lineNumber) + ": " + line);

            LOG_DEBUG("[CodeAnalyzer] Check if line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "] is empty");
            if (line.empty()) {
                LOG_DEBUG("[CodeAnalyzer] Line: " + std::to_string(lineNumber) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "] is empty");
                continue;
            }

            LOG_DEBUG("[CodeAnalyzer] Check if line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "] is a label");
            if (line.back() == ':') {
                LOG_DEBUG("[CodeAnalyzer] Line: " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "] is a label");
                std::string labelName = line.substr(0, line.length() - 1);
                labels_[labelName] = machine_code_.size();
                LOG_DEBUG("[CodeAnalyzer] Detected label " + labelName + " at address " + std::to_string(machine_code_.size()) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                continue;
            }

            LOG_DEBUG("[CodeAnalyzer] Tokenizing line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            std::vector<std::string> tokens = line_handler_->TokenizeLine(line);
            LOG_DEBUG("[CodeAnalyzer] Tokenized line " + std::to_string(lineNumber) + ": " + line + " into " + std::to_string(tokens.size()) + " tokens [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

            LOG_DEBUG("[CodeAnalyzer] Check if tokens are not empty for line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            if (!tokens.empty()) {
                LOG_DEBUG("[CodeAnalyzer] Tokens are not empty for line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                size_t size_before = machine_code_.size();
                LOG_DEBUG("[CodeAnalyzer] Machine code size before assembling instruction: " + std::to_string(size_before) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                if (!instructions_assembler_core_->AssembleInstruction(tokens)) {
                    LOG_ERROR("[CodeAnalyzer] Error on line " + std::to_string(lineNumber) + ": [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                    tools::PrintRedErrorMessage("Error on line " + std::to_string(lineNumber));
                    return false;
                }
            }
            LOG_DEBUG("[CodeAnalyzer] Finished processing line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenOKMessage("Finished processing line " + std::to_string(lineNumber) + ": " + line);
        }

        return true;
    }

    bool CodeAnalyzer::Tokenize(std::ifstream& file, std::string& line) {
        int lineNumber = 0;
        while (std::getline(file, line)) {
            ++lineNumber;
            LOG_DEBUG("[CodeAnalyzer] Processing line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            line = line_handler_->CleanLineWhitespaces(line);
            tools::PrintGreenOKMessage("Cleaned line " + std::to_string(lineNumber) + ": " + line);
            line = line_handler_->RemoveLineComments(line);
            tools::PrintGreenOKMessage("Removed comments from line " + std::to_string(lineNumber) + ": " + line);

            LOG_DEBUG("[CodeAnalyzer] Check if line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "] is empty or a label");
            if (line.empty() || line.back() == ':') {
                LOG_DEBUG("[CodeAnalyzer] Line: " + std::to_string(lineNumber) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "] is empty or a label");
                continue;
            }

            LOG_DEBUG("[CodeAnalyzer] Tokenizing line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            std::vector<std::string> tokens = line_handler_->TokenizeLine(line);
            if (!tokens.empty()) {
                try {
                    LOG_DEBUG("[CodeAnalyzer] Assembling instruction for line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                    instructions_assembler_core_->AssembleInstruction(tokens);
                } catch (const std::exception& e) {
                    LOG_ERROR("[CodeAnalyzer] Error on line " + std::to_string(lineNumber) + ": " + e.what() + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                    tools::PrintRedErrorMessage("Error on line " + std::to_string(lineNumber) + ": " + e.what());
                    return false;
                }
            }
            LOG_DEBUG("[CodeAnalyzer] Finished processing line " + std::to_string(lineNumber) + ": " + line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenOKMessage("Finished processing line " + std::to_string(lineNumber) + ": " + line);
        }

        return true;
    }

    bool CodeAnalyzer::ResolveLabelReferences() {
        for (auto& ref : label_references_) {
            LOG_TRACE("[AssemblyEngine] Resolving label reference: " + ref.second + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            if (labels_.find(ref.second) != labels_.end()) {
                machine_code_[ref.first] = static_cast<uint8_t>(labels_[ref.second]);
                LOG_DEBUG("[AssemblyEngine] Resolved label " + ref.second + " to address " + std::to_string(labels_[ref.second]) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            } else {
                LOG_ERROR("[AssemblyEngine] Undefined label: " + ref.second + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                tools::PrintRedErrorMessage("Undefined label: " + ref.second);
                return false;
            }
        }

        return true;
    }

}  // namespace assembly_engine