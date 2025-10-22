#include "AssemblyEngine.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Tools.h"
#include "spdlog/spdlog.h"

namespace assembly_engine {

    AssemblyEngine::AssemblyEngine()
        : file_handler_{std::make_unique<FileHandler>()},
          line_handler_{std::make_unique<CharacterStringLineHandler>()},
          machine_code_{},
          labels_{},
          label_references_{},
          instructions_assembler_core_{std::make_unique<InstructionsAssemblerCore>(machine_code_, label_references_)},
          code_analyzer_{std::make_unique<CodeAnalyzer>(machine_code_, labels_, label_references_)} {}

    /* For testing purposes */
    AssemblyEngine::AssemblyEngine(std::unique_ptr<IFileHandler> file_handler, std::unique_ptr<ICharacterStringLineHandler> line_handler,
                                   std::unique_ptr<IInstructionsAssemblerCore> instructions_assembler_core)
        : file_handler_{std::move(file_handler)},
          line_handler_{std::move(line_handler)},
          machine_code_{},
          labels_{},
          label_references_{},
          instructions_assembler_core_{std::move(instructions_assembler_core)},
          code_analyzer_{std::make_unique<CodeAnalyzer>(machine_code_, labels_, label_references_)} {}

    bool AssemblyEngine::Assemble(const std::string& input_file, const std::string& output_file) {
        spdlog::trace("[AssemblyEngine] Assemble() called with input_file: {0}, output_file: {1} [{2}:{3}]", input_file, output_file, __FILENAME__, __LINE__);

        spdlog::trace("[AssemblyEngine] Trying to open input file: {0} [{1}:{2}]", input_file, __FILENAME__, __LINE__);
        if (!file_handler_->OpenFileToRead(input_file)) {
            tools::PrintRedErrorMessage("Failed to open input file: " + input_file);
            spdlog::error("[AssemblyEngine] Failed to open input file: {0} [{1}:{2}]", input_file, __FILENAME__, __LINE__);
            return false;
        }
        spdlog::trace("[AssemblyEngine] Successfully opened input file: {0} [{1}:{2}]", input_file, __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Successfully opened input file: " + input_file);

        spdlog::debug("[AssemblyEngine] Get file stream [{0}:{1}]", __FILENAME__, __LINE__);
        std::ifstream& file = file_handler_->GetFileToReadStream();

        spdlog::trace("[AssemblyEngine] Clear line and reset linse number [{0}:{1}]", __FILENAME__, __LINE__);
        std::string line{};
        int lineNumber = 0;

        tools::PrintGreenOKMessage("Starting assembly process...");
        spdlog::trace("[AssemblyEngine] Starting assembly process... [{0}:{1}]", __FILENAME__, __LINE__);
        spdlog::debug("[AssemblyEngine] First pass - label detection... [{0}:{1}]", __FILENAME__, __LINE__);
        if (!code_analyzer_->DetectLabels(file, line)) {
            spdlog::error("[AssemblyEngine] Label detection failed [{0}:{1}]", __FILENAME__, __LINE__);
            return false;
        }
        spdlog::debug("[AssemblyEngine] Labels detected: {0} [{1}:{2}]", labels_.size(), __FILENAME__, __LINE__);

        // Reset for the second pass
        spdlog::trace("[AssemblyEngine] Reset file stream and internal states for second pass [{0}:{1}]", __FILENAME__, __LINE__);
        spdlog::debug("[AssemblyEngine] Clearing file stream state [{0}:{1}]", __FILENAME__, __LINE__);
        file.clear();
        spdlog::debug("[AssemblyEngine] Seek to beginning of file [{0}:{1}]", __FILENAME__, __LINE__);
        file.seekg(0, std::ios::beg);
        spdlog::debug("[AssemblyEngine] Clearing machine code and reset line number [{0}:{1}]", __FILENAME__, __LINE__);
        machine_code_.clear();
        spdlog::debug("[AssemblyEngine] Clearing line number [{0}:{1}]", __FILENAME__, __LINE__);
        lineNumber = 0;

        // Second pass - actual assembly
        spdlog::debug("[AssemblyEngine] Second pass - actual assembly... [{0}:{1}]", __FILENAME__, __LINE__);
        if (!code_analyzer_->Tokenize(file, line)) {
            spdlog::error("[AssemblyEngine] Tokenization and assembly failed [{0}:{1}]", __FILENAME__, __LINE__);
            return false;
        }
        // while (std::getline(file, line)) {
        //     lineNumber++;
        //     spdlog::debug("[AssemblyEngine] Processing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
        //     line = line_handler_->CleanLineWhitespaces(line);
        //     tools::PrintGreenOKMessage("Cleaned line " + std::to_string(lineNumber) + ": " + line);
        //     line = line_handler_->RemoveLineComments(line);
        //     tools::PrintGreenOKMessage("Removed comments from line " + std::to_string(lineNumber) + ": " + line);

        //     spdlog::debug("[AssemblyEngine] Check if line {0}: {1} [{2}:{3}] is empty or a label", lineNumber, line, __FILENAME__, __LINE__);
        //     if (line.empty() || line.back() == ':') {
        //         spdlog::debug("[AssemblyEngine] Line: {0} [{1}:{2}] is empty or a label", lineNumber, line, __FILENAME__, __LINE__);
        //         continue;
        //     }

        //     spdlog::debug("[AssemblyEngine] Tokenizing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
        //     std::vector<std::string> tokens = line_handler_->TokenizeLine(line);
        //     if (!tokens.empty()) {
        //         try {
        //             spdlog::debug("[AssemblyEngine] Assembling instruction for line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
        //             instructions_assembler_core_->AssembleInstruction(tokens);
        //         } catch (const std::exception& e) {
        //             spdlog::error("[AssemblyEngine] Error on line {0}: {1} [{2}:{3}]", lineNumber, e.what(), __FILENAME__, __LINE__);
        //             tools::PrintRedErrorMessage("Error on line " + std::to_string(lineNumber) + ": " + e.what());
        //             return false;
        //         }
        //     }
        //     spdlog::debug("[AssemblyEngine] Finished processing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
        //     tools::PrintGreenOKMessage("Finished processing line " + std::to_string(lineNumber) + ": " + line);
        // }

        // Resolve references to labels
        spdlog::debug("[AssemblyEngine] Resolving label references... [{0}:{1}]", __FILENAME__, __LINE__);
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

        spdlog::debug("[AssemblyEngine] Closing file stream [{0}:{1}]", __FILENAME__, __LINE__);
        file.close();

        spdlog::debug("[AssemblyEngine] Assembly process completed successfully [{0}:{1}]", output_file, __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Assembly process completed successfully.");
        return true;
    }

    void AssemblyEngine::PrintMachineCode() const {
        spdlog::trace("[AssemblyEngine] PrintMachineCode() called [{0}:{1}]", __FILENAME__, __LINE__);

        std::cout << "Machine Code:" << std::endl;
        for (size_t i = 0; i < machine_code_.size(); i++) {
            std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(machine_code_[i]);
            if ((i + 1) % 8 == 0) {
                std::cout << std::endl;
            } else {
                std::cout << " ";
            }
        }
        if (machine_code_.size() % 8 != 0) {
            std::cout << std::endl;
        }
    }

    bool AssemblyEngine::SaveMachineCodeToFile(const std::string& output_file) const {
        spdlog::trace("[AssemblyEngine] SaveMachineCodeToFile() called with output_file: {0} [{1}:{2}]", output_file, __FILENAME__, __LINE__);

        spdlog::trace("[AssemblyEngine] Trying to open output file: {0} [{1}:{2}]", output_file, __FILENAME__, __LINE__);
        if (!file_handler_->OpenFileToWrite(output_file)) {
            tools::PrintRedErrorMessage("Failed to open output file: " + output_file);
            spdlog::error("[AssemblyEngine] Failed to open output file: {0} [{1}:{2}]", output_file, __FILENAME__, __LINE__);
            return false;
        }
        spdlog::trace("[AssemblyEngine] Successfully opened output file: {0} [{1}:{2}]", output_file, __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Successfully opened output file: " + output_file);

        spdlog::debug("[AssemblyEngine] Get file to write stream [{0}:{1}]", __FILENAME__, __LINE__);
        std::ofstream& file = file_handler_->GetFileToWriteStream();

        if (!file) {
            tools::PrintRedErrorMessage("Output file stream is not valid.");
            spdlog::error("[AssemblyEngine] Output file stream is not valid. [{0}:{1}]", __FILENAME__, __LINE__);
            return false;
        }

        for (uint8_t byte : machine_code_) {
            file << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << std::endl;
        }
        file.close();
        tools::PrintGreenOKMessage("Machine code saved to file: " + output_file);

        return true;
    }

}  // namespace assembly_engine