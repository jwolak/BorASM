#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace assembly_engine {
    class CharacterStringLineHandler {
      public:
        CharacterStringLineHandler();
        std::string CleanLineWhitespaces(const std::string& line_to_be_processed) const;
        std::string RemoveLineComments(const std::string& line_to_be_processed) const;
        bool IsNumber(const std::string& string_number) const;
        uint8_t ConvertStringToNumber(const std::string& string_number) const;
        std::vector<std::string> TokenizeLine(const std::string& line_to_be_processed) const;
    };
}  // namespace assembly_engine