#include "AssemblyEngine.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Tools.h"
#include "EquinoxLogger.hpp"

namespace assembly_engine {

    AssemblyEngine::AssemblyEngine()
        : file_handler_{std::make_unique<FileHandler>()},
          machine_code_{},
          labels_{},
          label_references_{},
          code_analyzer_{std::make_unique<CodeAnalyzer>(machine_code_, labels_, label_references_)} {}

    /* For testing purposes */
    AssemblyEngine::AssemblyEngine(std::unique_ptr<IFileHandler> file_handler, std::unique_ptr<ICodeAnalyzer> code_analyzer, std::vector<uint8_t> machine_code)
        : file_handler_{std::move(file_handler)}, machine_code_{machine_code}, labels_{}, label_references_{}, code_analyzer_{std::move(code_analyzer)} {}

    bool AssemblyEngine::Assemble(const std::string& input_file, const std::string& output_file) {
        LOG_TRACE("[AssemblyEngine] Assemble() called with input_file: " + input_file + ", output_file: " + output_file + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_TRACE("[AssemblyEngine] Trying to open input file: " + input_file + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (!file_handler_->OpenFileToRead(input_file)) {
            tools::PrintRedErrorMessage("Failed to open input file: " + input_file);
            LOG_ERROR("[AssemblyEngine] Failed to open input file: " + input_file + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return false;
        }
        LOG_TRACE("[AssemblyEngine] Successfully opened input file: " + input_file + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Successfully opened input file: " + input_file);

        LOG_DEBUG("[AssemblyEngine] Get file stream [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        std::ifstream& file = file_handler_->GetFileToReadStream();

        LOG_TRACE("[AssemblyEngine] Clear line and reset line number [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        std::string line{};

        tools::PrintGreenOKMessage("Starting assembly process...");
        LOG_TRACE("[AssemblyEngine] Starting assembly process... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        LOG_DEBUG("[AssemblyEngine] First pass - label detection... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (!code_analyzer_->DetectLabels(file, line)) {
            LOG_ERROR("[AssemblyEngine] Label detection failed [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return false;
        }
        LOG_DEBUG("[AssemblyEngine] Labels detected: " + std::to_string(labels_.size()) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        // Reset for the second pass
        LOG_TRACE("[AssemblyEngine] Reset file stream and internal states for second pass [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        LOG_DEBUG("[AssemblyEngine] Clearing file stream state [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        file.clear();
        LOG_DEBUG("[AssemblyEngine] Seek to beginning of file [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        file.seekg(0, std::ios::beg);
        LOG_DEBUG("[AssemblyEngine] Clearing machine code and reset line number [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        machine_code_.clear();
        LOG_DEBUG("[AssemblyEngine] Clearing line number [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        // Second pass - actual assembly
        LOG_DEBUG("[AssemblyEngine] Second pass - actual assembly... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (!code_analyzer_->Tokenize(file, line)) {
            LOG_ERROR("[AssemblyEngine] Tokenization and assembly failed [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return false;
        }

        // Resolve references to labels
        LOG_DEBUG("[AssemblyEngine] Resolving label references... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (!code_analyzer_->ResolveLabelReferences()) {
            LOG_ERROR("[AssemblyEngine] Resolving label references failed [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return false;
        }

        LOG_DEBUG("[AssemblyEngine] Closing file stream [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        file.close();

        LOG_DEBUG("[AssemblyEngine] Assembly process completed successfully [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Assembly process completed successfully.");
        return true;
    }

    void AssemblyEngine::PrintMachineCode() const {
        LOG_TRACE("[AssemblyEngine] PrintMachineCode() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

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
        LOG_TRACE("[AssemblyEngine] SaveMachineCodeToFile() called with output_file: " + output_file + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_TRACE("[AssemblyEngine] Trying to open output file: " + output_file + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (!file_handler_->OpenFileToWrite(output_file)) {
            tools::PrintRedErrorMessage("Failed to open output file: " + output_file);
            LOG_ERROR("[AssemblyEngine] Failed to open output file: " + output_file + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return false;
        }
        LOG_TRACE("[AssemblyEngine] Successfully opened output file: " + output_file + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Successfully opened output file: " + output_file);

        LOG_DEBUG("[AssemblyEngine] Get file to write stream [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        std::ofstream& file = file_handler_->GetFileToWriteStream();

        for (uint8_t byte : machine_code_) {
            file << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << std::endl;
        }
        file.close();
        tools::PrintGreenOKMessage("Machine code saved to file: " + output_file);

        return true;
    }

}  // namespace assembly_engine