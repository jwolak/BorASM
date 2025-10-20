#pragma once

#include <cstdint>
#include <string>

namespace assembly_engine {
    class CharacterStringLineHandler {
      public:
        CharacterStringLineHandler();
        std::string CleanLineWhitespaces(const std::string& line) const;
        std::string RemoveLineComments(const std::string& line) const;
        bool IsNumber(const std::string& string_number) const;
        uint8_t ConvertStringToNumber(const std::string& string_number) const;
    };
}  // namespace assembly_engine