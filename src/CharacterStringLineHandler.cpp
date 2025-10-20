#include "CharacterStringLineHandler.h"

#include <cstdint>
#include <string>

namespace assembly_engine {

    CharacterStringLineHandler::CharacterStringLineHandler() {}

    std::string CharacterStringLineHandler::CleanLineWhitespaces(const std::string& line) const {
        std::string clean = line;
        clean.erase(0, clean.find_first_not_of(" \t\r\n"));
        clean.erase(clean.find_last_not_of(" \t\r\n") + 1);
        return clean;
    }

    std::string CharacterStringLineHandler::RemoveLineComments(const std::string& line) const {
        std::string clean = line;
        size_t commentPos = clean.find("//");
        if (commentPos != std::string::npos) {
            clean = clean.substr(0, commentPos);
        }

        commentPos = clean.find(";");
        if (commentPos != std::string::npos) {
            clean = clean.substr(0, commentPos);
        }

        return clean;
    }

    bool CharacterStringLineHandler::IsNumber(const std::string& string_number) const {
        if (string_number.empty()) return false;

        size_t start = 0;
        if (string_number[0] == '#') start = 1;  // Immediate value

        // Hex number
        if (string_number.length() > start + 2 && string_number.substr(start, 2) == "0x") {
            for (size_t i = start + 2; i < string_number.length(); i++) {
                if (!std::isxdigit(string_number[i])) return false;
            }
            return true;
        }

        // Decimal number
        for (size_t i = start; i < string_number.length(); i++) {
            if (!std::isdigit(string_number[i])) return false;
        }
        return true;
    }

    uint8_t CharacterStringLineHandler::ConvertStringToNumber(const std::string& string_number) const {
        std::string num = string_number;
        if (num[0] == '#') num = num.substr(1);  // Usuń #

        if (num.length() > 2 && num.substr(0, 2) == "0x") {
            return static_cast<uint8_t>(std::stoul(num, nullptr, 16));
        }
        return static_cast<uint8_t>(std::stoul(num));
    }

}  // namespace assembly_engine