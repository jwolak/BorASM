#pragma once

#include <optional>
#include <string>

namespace cmd {
    class IArgumentsParserLogic {
      public:
        virtual ~IArgumentsParserLogic() = default;
        virtual void PrintHelp() const = 0;
        virtual void PrintVersionInfo() const = 0;
        virtual bool EnableDebugMode() const = 0;
        virtual std::optional<std::string> GetInputFileName(const char* optarg) const = 0;
        virtual std::optional<std::string> GetOutputFileName(const char* optarg) const = 0;
        virtual void ListAvailableInstructions() const = 0;
    };
}  // namespace cmd