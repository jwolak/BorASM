#include "AssemblyEngine.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace assembly_engine {

    AssemblyEngine::AssemblyEngine() {}
    bool AssemblyEngine::Assemble(const std::string& input_file, const std::string& output_file) {
        std::ifstream file(input_file);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + input_file);
        }

        std::string line;
        int lineNumber = 0;

        // Pierwsza przebiega - znajdź labels
        while (std::getline(file, line)) {
            lineNumber++;
            line = cleanLine(line);

            if (line.empty()) continue;

            // Sprawdź czy to label
            if (line.back() == ':') {
                std::string labelName = line.substr(0, line.length() - 1);
                labels[labelName] = machineCode.size();
                continue;
            }

            // Tymczasowo asembluj aby poznać rozmiar
            std::vector<std::string> tokens = tokenize(line);
            if (!tokens.empty()) {
                try {
                    size_t sizeBefore = machineCode.size();
                    assembleInstruction(tokens);
                } catch (const std::exception& e) {
                    std::cerr << "Error on line " << lineNumber << ": " << e.what() << std::endl;
                }
            }
        }

        // Reset dla drugiej przebiegłości
        file.clear();
        file.seekg(0, std::ios::beg);
        machineCode.clear();
        lineNumber = 0;

        // Druga przebiega - właściwa asemblacja
        while (std::getline(file, line)) {
            lineNumber++;
            line = cleanLine(line);

            if (line.empty() || line.back() == ':') continue;

            std::vector<std::string> tokens = tokenize(line);
            if (!tokens.empty()) {
                try {
                    assembleInstruction(tokens);
                } catch (const std::exception& e) {
                    std::cerr << "Error on line " << lineNumber << ": " << e.what() << std::endl;
                }
            }
        }

        // Rozwiąż referencje do labels
        for (auto& ref : labelReferences) {
            if (labels.find(ref.second) != labels.end()) {
                machineCode[ref.first] = static_cast<uint8_t>(labels[ref.second]);
            } else {
                std::cerr << "Undefined label: " << ref.second << std::endl;
            }
        }

        file.close();
    }
}  // namespace assembly_engine