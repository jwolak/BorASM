#include "CharacterStringLineHandler.h"

#include <cstdint>
#include <sstream>
#include <string>

#include "Tools.h"
#include "EquinoxLogger.hpp"

namespace assembly_engine {

    CharacterStringLineHandler::CharacterStringLineHandler() {}

    std::string CharacterStringLineHandler::CleanLineWhitespaces(const std::string& line_to_be_processed) const {
        LOG_TRACE("[CharacterStringLineHandler] CleanLineWhitespaces() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Cleaning line whitespaces...");

        LOG_DEBUG("[CharacterStringLineHandler] Cleaning line whitespaces for line: " + line_to_be_processed + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        std::string cleaned_line = line_to_be_processed;
        cleaned_line.erase(0, cleaned_line.find_first_not_of(" \t\r\n"));
        cleaned_line.erase(cleaned_line.find_last_not_of(" \t\r\n") + 1);

        LOG_DEBUG("[CharacterStringLineHandler] Cleaned line: " + cleaned_line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        return cleaned_line;
    }

    std::string CharacterStringLineHandler::RemoveLineComments(const std::string& line_to_be_processed) const {
        LOG_TRACE("[CharacterStringLineHandler] RemoveLineComments() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Removing comments...");

        LOG_DEBUG("[CharacterStringLineHandler] Removing line comments for line: " + line_to_be_processed + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        std::string cleaned_line = line_to_be_processed;
        size_t commentPos = cleaned_line.find("//");
        if (commentPos != std::string::npos) {
            cleaned_line = cleaned_line.substr(0, commentPos);
        }
        LOG_DEBUG("[CharacterStringLineHandler] ['//'] Comment removed from line: " + cleaned_line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        tools::PrintGreenOKMessage("Removing block comments...");
        LOG_DEBUG("[CharacterStringLineHandler] Removing block comments for line: " + line_to_be_processed + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        commentPos = cleaned_line.find(";");
        if (commentPos != std::string::npos) {
            cleaned_line = cleaned_line.substr(0, commentPos);
        }
        LOG_DEBUG("[CharacterStringLineHandler] [';'] Comment removed from line: " + cleaned_line + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        tools::PrintGreenOKMessage("Block comments removed.");
        LOG_DEBUG("[CharacterStringLineHandler] Block comments removed for line: " + line_to_be_processed + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        return cleaned_line;
    }

    bool CharacterStringLineHandler::IsNumber(const std::string& string_number) const {
        LOG_TRACE("[CharacterStringLineHandler] IsNumber() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Checking if string is a number...");

        LOG_TRACE("[CharacterStringLineHandler] Checking if string: " + string_number + " is a number [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (string_number.empty()) {
            LOG_DEBUG("[CharacterStringLineHandler] String is empty, not a number [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintRedErrorMessage("String is empty, not a number.");
            return false;
        }

        LOG_DEBUG("[CharacterStringLineHandler] Checking if immediate value... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        size_t start = 0;
        if (string_number[0] == '#') {
            LOG_DEBUG("[CharacterStringLineHandler] Immediate value detected [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            start = 1;  // Immediate value
        }

        LOG_DEBUG("[CharacterStringLineHandler] Checking if hex value... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (string_number.length() > start + 2 && string_number.substr(start, 2) == "0x") {
            for (size_t i = start + 2; i < string_number.length(); i++) {
                if (!std::isxdigit(string_number[i])) {
                    LOG_DEBUG("[CharacterStringLineHandler] Invalid hex digit found: " + std::string(1, string_number[i]) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                    tools::PrintRedErrorMessage("Invalid hex digit found.");
                    return false;
                }
                LOG_TRACE("[CharacterStringLineHandler] Valid hex digit: " + std::string(1, string_number[i]) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            }
            LOG_DEBUG("[CharacterStringLineHandler] Hex value is valid [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenOKMessage("Hex value is valid.");
            return true;
        }

        LOG_DEBUG("[CharacterStringLineHandler] Checking if decimal value... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        for (size_t i = start; i < string_number.length(); i++) {
            if (!std::isdigit(string_number[i])) {
                LOG_DEBUG("[CharacterStringLineHandler] Invalid decimal digit found: " + std::string(1, string_number[i]) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                return false;
            }
        }
        LOG_DEBUG("[CharacterStringLineHandler] Decimal value is valid [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Decimal value is valid.");
        return true;
    }

    uint8_t CharacterStringLineHandler::ConvertStringToNumber(const std::string& string_number) const {
        LOG_TRACE("[CharacterStringLineHandler] ConvertStringToNumber() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Converting string to number...");

        LOG_DEBUG("[CharacterStringLineHandler] Converting string: " + string_number + " to number [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        std::string num = string_number;
        LOG_DEBUG("[CharacterStringLineHandler] Initial string: " + num + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (num[0] == '#') {
            LOG_DEBUG("[CharacterStringLineHandler] Removing '#' from string [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            num = num.substr(1);  // Remove '#' for immediate values
            LOG_DEBUG("[CharacterStringLineHandler] String after removing '#': " + num + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        }

        LOG_DEBUG("[CharacterStringLineHandler] Checking if hex format... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (num.length() > 2 && num.substr(0, 2) == "0x") {
            LOG_DEBUG("[CharacterStringLineHandler] Hex format detected, converting... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenOKMessage("Converting hex string to number...");
            return static_cast<uint8_t>(std::stoul(num, nullptr, 16));
        }
        LOG_DEBUG("[CharacterStringLineHandler] Decimal format detected, converting... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Converting decimal string to number...");

        LOG_DEBUG("[CharacterStringLineHandler] String number after processing: " + num + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("String converted to number successfully.");
        return static_cast<uint8_t>(std::stoul(num));
    }

    std::vector<std::string> CharacterStringLineHandler::TokenizeLine(const std::string& line_to_be_processed) const {
        LOG_TRACE("[CharacterStringLineHandler] TokenizeLine() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Tokenizing line...");

        LOG_DEBUG("[CharacterStringLineHandler] Tokenizing line: " + line_to_be_processed + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        std::vector<std::string> tokens;
        std::istringstream iss(line_to_be_processed);
        std::string token;

        LOG_DEBUG("[CharacterStringLineHandler] Splitting line into tokens... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        while (iss >> token) {
            if (!token.empty() && token.back() == ',') {
                LOG_DEBUG("[CharacterStringLineHandler] Removing trailing comma ',' from token: " + token + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                token.pop_back();
                LOG_DEBUG("[CharacterStringLineHandler] Token after removing comma: " + token + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            }
            if (!token.empty()) {
                LOG_DEBUG("[CharacterStringLineHandler] Adding token: " + token + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                tokens.push_back(token);
            }
        }

        LOG_DEBUG("[CharacterStringLineHandler] Tokenization complete. Total tokens: " + std::to_string(tokens.size()) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Line tokenized successfully.");
        return tokens;
    }

}  // namespace assembly_engine