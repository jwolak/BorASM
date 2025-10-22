#include "CodeAnalyzer.h"

#include <fstream>
#include <map>
#include <memory>
#include <string>

#include "CharacterStringLineHandler.h"
#include "InstructionsAssemblerCore.h"
#include "Tools.h"
#include "spdlog/spdlog.h"

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
            spdlog::debug("[CodeAnalyzer] Processing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            line = line_handler_->CleanLineWhitespaces(line);
            tools::PrintGreenOKMessage("Cleaned line " + std::to_string(lineNumber) + ": " + line);
            line = line_handler_->RemoveLineComments(line);
            tools::PrintGreenOKMessage("Removed comments from line " + std::to_string(lineNumber) + ": " + line);

            spdlog::debug("[CodeAnalyzer] Check if line {0}: {1} [{2}:{3}] is empty", lineNumber, line, __FILENAME__, __LINE__);
            if (line.empty()) {
                spdlog::debug("[CodeAnalyzer] Line: {0} [{1}:{2}] is empty", lineNumber, line, __FILENAME__, __LINE__);
                continue;
            }

            spdlog::debug("[CodeAnalyzer] Check if line {0}: {1} [{2}:{3}] is a label", lineNumber, line, __FILENAME__, __LINE__);
            if (line.back() == ':') {
                spdlog::debug("[CodeAnalyzer] Line: {0} [{1}:{2}] is a label", lineNumber, line, __FILENAME__, __LINE__);
                std::string labelName = line.substr(0, line.length() - 1);
                labels_[labelName] = machine_code_.size();
                spdlog::debug("[CodeAnalyzer] Detected label {0} at address {1} [{2}:{3}]", labelName, machine_code_.size(), __FILENAME__, __LINE__);
                continue;
            }

            spdlog::debug("[CodeAnalyzer] Tokenizing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            std::vector<std::string> tokens = line_handler_->TokenizeLine(line);
            spdlog::debug("[CodeAnalyzer] Tokenized line {0}: {1} into {2} tokens [{3}:{4}]", lineNumber, line, tokens.size(), __FILENAME__, __LINE__);

            spdlog::debug("[CodeAnalyzer] Check if tokens are not empty for line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            if (!tokens.empty()) {
                spdlog::debug("[CodeAnalyzer] Tokens are not empty for line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
                try {
                    size_t size_before = machine_code_.size();
                    spdlog::debug("[CodeAnalyzer] Machine code size before assembling instruction: {0} [{1}:{2}]", size_before, __FILENAME__, __LINE__);
                    instructions_assembler_core_->AssembleInstruction(tokens);
                } catch (const std::exception& e) {
                    spdlog::error("[CodeAnalyzer] Error on line {0}: {1} [{2}:{3}]", lineNumber, e.what(), __FILENAME__, __LINE__);
                    tools::PrintRedErrorMessage("Error on line " + std::to_string(lineNumber) + ": " + e.what());
                    return false;
                }
            }
            spdlog::debug("[CodeAnalyzer] Finished processing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            tools::PrintGreenOKMessage("Finished processing line " + std::to_string(lineNumber) + ": " + line);
        }

        return true;
    }

    bool CodeAnalyzer::Tokenize(std::ifstream& file, std::string& line) {
        int lineNumber = 0;
        while (std::getline(file, line)) {
            ++lineNumber;
            spdlog::debug("[CodeAnalyzer] Processing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            line = line_handler_->CleanLineWhitespaces(line);
            tools::PrintGreenOKMessage("Cleaned line " + std::to_string(lineNumber) + ": " + line);
            line = line_handler_->RemoveLineComments(line);
            tools::PrintGreenOKMessage("Removed comments from line " + std::to_string(lineNumber) + ": " + line);

            spdlog::debug("[CodeAnalyzer] Check if line {0}: {1} [{2}:{3}] is empty or a label", lineNumber, line, __FILENAME__, __LINE__);
            if (line.empty() || line.back() == ':') {
                spdlog::debug("[CodeAnalyzer] Line: {0} [{1}:{2}] is empty or a label", lineNumber, line, __FILENAME__, __LINE__);
                continue;
            }

            spdlog::debug("[CodeAnalyzer] Tokenizing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            std::vector<std::string> tokens = line_handler_->TokenizeLine(line);
            if (!tokens.empty()) {
                try {
                    spdlog::debug("[CodeAnalyzer] Assembling instruction for line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
                    instructions_assembler_core_->AssembleInstruction(tokens);
                } catch (const std::exception& e) {
                    spdlog::error("[CodeAnalyzer] Error on line {0}: {1} [{2}:{3}]", lineNumber, e.what(), __FILENAME__, __LINE__);
                    tools::PrintRedErrorMessage("Error on line " + std::to_string(lineNumber) + ": " + e.what());
                    return false;
                }
            }
            spdlog::debug("[CodeAnalyzer] Finished processing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            tools::PrintGreenOKMessage("Finished processing line " + std::to_string(lineNumber) + ": " + line);
        }

        return true;
    }

    bool CodeAnalyzer::ResolveLabelReferences() {
        for (auto& ref : label_references_) {
            spdlog::trace("[AssemblyEngine] Resolving label reference: {0} [{1}:{2}]", ref.second, __FILENAME__, __LINE__);
            if (labels_.find(ref.second) != labels_.end()) {
                machine_code_[ref.first] = static_cast<uint8_t>(labels_[ref.second]);
                spdlog::debug("[AssemblyEngine] Resolved label {0} to address {1} [{2}:{3}]", ref.second, labels_[ref.second], __FILENAME__, __LINE__);
            } else {
                spdlog::error("[AssemblyEngine] Undefined label: {0} [{1}:{2}]", ref.second, __FILENAME__, __LINE__);
                tools::PrintRedErrorMessage("Undefined label: " + ref.second);
                return false;
            }
        }

        return true;
    }

}  // namespace assembly_engine