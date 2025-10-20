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

    AssemblyEngine::AssemblyEngine() : file_handler_{std::make_unique<FileHandler>()} {}

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

        std::ifstream& file = file_handler_->GetFileStream();
        std::string line;
        int lineNumber = 0;

        // Pierwsza przebiega - znajdź labels
        while (std::getline(file, line)) {
            // lineNumber++;
            // line = cleanLine(line);

            // if (line.empty()) continue;

            // // Sprawdź czy to label
            // if (line.back() == ':') {
            //     std::string labelName = line.substr(0, line.length() - 1);
            //     labels[labelName] = machineCode.size();
            //     continue;
            // }

            // // Tymczasowo asembluj aby poznać rozmiar
            // std::vector<std::string> tokens = tokenize(line);
            // if (!tokens.empty()) {
            //     try {
            //         size_t sizeBefore = machineCode.size();
            //         assembleInstruction(tokens);
            //     } catch (const std::exception& e) {
            //         std::cerr << "Error on line " << lineNumber << ": " << e.what() << std::endl;
            //     }
            // }
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