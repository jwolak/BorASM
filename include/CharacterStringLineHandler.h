#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "ICharacterStringLineHandler.h"

namespace assembly_engine {
    class CharacterStringLineHandler : public ICharacterStringLineHandler {
      public:
        CharacterStringLineHandler();
        std::string CleanLineWhitespaces(const std::string& line_to_be_processed) const override;
        std::string RemoveLineComments(const std::string& line_to_be_processed) const override;
        bool IsNumber(const std::string& string_number) const override;
        uint8_t ConvertStringToNumber(const std::string& string_number) const override;
        std::vector<std::string> TokenizeLine(const std::string& line_to_be_processed) const override;
    };
}  // namespace assembly_engine