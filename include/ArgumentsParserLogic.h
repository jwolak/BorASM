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
    };
}  // namespace cmd