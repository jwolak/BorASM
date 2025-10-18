#pragma once

#include <optional>
#include <string>

namespace cmd {
    class ArgumentsParserLogic {
      public:
        ArgumentsParserLogic();
        void PrintHelp() const;
        void PrintVersionInfo() const;
        bool EnableDebugMode() const;
        std::optional<std::string> GetInputFileName(const char* optarg) const;
        std::optional<std::string> GetOutputFileName(const char* optarg) const;
        void ListAvailableInstructions() const;
    };
}  // namespace cmd