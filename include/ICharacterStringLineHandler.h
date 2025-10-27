#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace assembly_engine {
    class ICharacterStringLineHandler {
      public:
        virtual ~ICharacterStringLineHandler() = default;
        virtual std::string CleanLineWhitespaces(const std::string& line_to_be_processed) const = 0;
        virtual std::string RemoveLineComments(const std::string& line_to_be_processed) const = 0;
        virtual bool IsNumber(const std::string& string_number) const = 0;
        virtual uint8_t ConvertStringToNumber(const std::string& string_number) const = 0;
        virtual std::vector<std::string> TokenizeLine(const std::string& line_to_be_processed) const = 0;
    };
}  // namespace assembly_engine