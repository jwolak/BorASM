#include "AssemblyEngine.h"

#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Tools.h"

namespace assembly_engine {

    AssemblyEngine::AssemblyEngine()
        : file_handler_{std::make_unique<FileHandler>()},
          line_handler_{std::make_unique<CharacterStringLineHandler>()},
          machineCode{},
          labels{},
          labelReferences{} {}

    bool AssemblyEngine::Assemble(const std::string& input_file, const std::string& output_file) {
        spdlog::trace("[AssemblyEngine] Assemble() called with input_file: {0}, output_file: {1} [{2}:{3}]", input_file, output_file, __FILENAME__, __LINE__);

        spdlog::trace("[AssemblyEngine] Trying to open input file: {0} [{1}:{2}]", input_file, __FILENAME__, __LINE__);
        if (!file_handler_->OpenFile(input_file, std::ios::in | std::ios::binary)) {
            tools::PrintRedErrorMessage("Failed to open input file: " + input_file);
            spdlog::error("[AssemblyEngine] Failed to open input file: {0} [{1}:{2}]", input_file, __FILENAME__, __LINE__);
            return false;
        }
        spdlog::trace("[AssemblyEngine] Successfully opened input file: {0} [{1}:{2}]", input_file, __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Successfully opened input file: " + input_file);

        spdlog::debug("[AssemblyEngine] Get file stream [{0}:{1}]", __FILENAME__, __LINE__);
        std::ifstream& file = file_handler_->GetFileStream();

        if (!file) {
            tools::PrintRedErrorMessage("Input file stream is not valid.");
            spdlog::error("[AssemblyEngine] Input file stream is not valid. [{0}:{1}]", __FILENAME__, __LINE__);
            return false;
        }

        spdlog::trace("[AssemblyEngine] Clear line and reset linse number [{0}:{1}]", __FILENAME__, __LINE__);
        std::string line{};
        int lineNumber = 0;

        tools::PrintGreenOKMessage("Starting assembly process...");
        spdlog::trace("[AssemblyEngine] Starting assembly process... [{0}:{1}]", __FILENAME__, __LINE__);
        spdlog::debug("[AssemblyEngine] First pass - label detection... [{0}:{1}]", __FILENAME__, __LINE__);
        while (std::getline(file, line)) {
            ++lineNumber;
            spdlog::debug("[AssemblyEngine] Processing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            line = line_handler_->CleanLineWhitespaces(line);
            tools::PrintGreenOKMessage("Cleaned line " + std::to_string(lineNumber) + ": " + line);
            line = line_handler_->RemoveLineComments(line);
            tools::PrintGreenOKMessage("Removed comments from line " + std::to_string(lineNumber) + ": " + line);

            spdlog::debug("[AssemblyEngine] Check if line {0}: {1} [{2}:{3}] is empty", lineNumber, line, __FILENAME__, __LINE__);
            if (line.empty()) {
                spdlog::debug("[AssemblyEngine] Line: {0} [{1}:{2}] is empty", lineNumber, line, __FILENAME__, __LINE__);
                continue;
            }

            spdlog::debug("[AssemblyEngine] Check if line {0}: {1} [{2}:{3}] is a label", lineNumber, line, __FILENAME__, __LINE__);
            if (line.back() == ':') {
                spdlog::debug("[AssemblyEngine] Line: {0} [{1}:{2}] is a label", lineNumber, line, __FILENAME__, __LINE__);
                std::string labelName = line.substr(0, line.length() - 1);
                labels[labelName] = machineCode.size();
                continue;
            }

            spdlog::debug("[AssemblyEngine] Tokenizing line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            std::vector<std::string> tokens = line_handler_->TokenizeLine(line);
            spdlog::debug("[AssemblyEngine] Tokenized line {0}: {1} into {2} tokens [{3}:{4}]", lineNumber, line, tokens.size(), __FILENAME__, __LINE__);

            spdlog::debug("[AssemblyEngine] Check if tokens are not empty for line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
            if (!tokens.empty()) {
                spdlog::debug("[AssemblyEngine] Tokens are not empty for line {0}: {1} [{2}:{3}]", lineNumber, line, __FILENAME__, __LINE__);
                try {
                    size_t size_before = machineCode.size();
                    spdlog::debug("[AssemblyEngine] Machine code size before assembling instruction: {0} [{1}:{2}]", size_before, __FILENAME__, __LINE__);
                    // assembleInstruction(tokens);
                } catch (const std::exception& e) {
                    spdlog::error("[AssemblyEngine] Error on line {0}: {1} [{2}:{3}]", lineNumber, e.what(), __FILENAME__, __LINE__);
                    tools::PrintRedErrorMessage("Error on line " + std::to_string(lineNumber) + ": " + e.what());
                    return false;
                }
            }
        }

        // // Reset dla drugiej przebiegłości
        // file.clear();
        // file.seekg(0, std::ios::beg);
        // machineCode.clear();
        // lineNumber = 0;

        // // Druga przebiega - właściwa asemblacja
        // while (std::getline(file, line)) {
        //     lineNumber++;
        //     line = cleanLine(line);

        //     if (line.empty() || line.back() == ':') continue;

        //     std::vector<std::string> tokens = tokenize(line);
        //     if (!tokens.empty()) {
        //         try {
        //             assembleInstruction(tokens);
        //         } catch (const std::exception& e) {
        //             std::cerr << "Error on line " << lineNumber << ": " << e.what() << std::endl;
        //         }
        //     }
        // }

        // // Rozwiąż referencje do labels
        // for (auto& ref : labelReferences) {
        //     if (labels.find(ref.second) != labels.end()) {
        //         machineCode[ref.first] = static_cast<uint8_t>(labels[ref.second]);
        //     } else {
        //         std::cerr << "Undefined label: " << ref.second << std::endl;
        //     }
        // }

        // file.close();
    }
}  // namespace assembly_engine