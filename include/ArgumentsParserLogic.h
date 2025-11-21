#pragma once

#include <optional>
#include <string>

#include "IArgumentsParserLogic.h"

namespace cmd {
    class ArgumentsParserLogic : public IArgumentsParserLogic {
      public:
        ArgumentsParserLogic();
        void PrintHelp() const override;
        void PrintVersionInfo() const override;
        bool EnableDebugMode() const override;
        std::optional<std::string> GetInputFileName(const char* optarg) const override;
        std::optional<std::string> GetOutputFileName(const char* optarg) const override;
        void ListAvailableInstructions() const override;
        std::string SetInputFileAsOutputFileName(const std::string& input_file_name) const;
        bool CheckOutputFileNameIsNotSameAsInputFileName(const std::string& input_file_name, const std::string& output_file_name) const override;
    };
}  // namespace cmd