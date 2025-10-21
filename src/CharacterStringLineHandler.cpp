#include "CharacterStringLineHandler.h"

#include <cstdint>
#include <sstream>
#include <string>

#include "Tools.h"
#include "spdlog/spdlog.h"

namespace assembly_engine {

    CharacterStringLineHandler::CharacterStringLineHandler() {}

    std::string CharacterStringLineHandler::CleanLineWhitespaces(const std::string& line_to_be_processed) const {
        spdlog::trace("[CharacterStringLineHandler] CleanLineWhitespaces() called [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Cleaning line whitespaces...");

        spdlog::debug("[CharacterStringLineHandler] Cleaning line whitespaces for line: {0} [{1}:{2}]", line_to_be_processed, __FILENAME__, __LINE__);
        std::string cleaned_line = line_to_be_processed;
        cleaned_line.erase(0, cleaned_line.find_first_not_of(" \t\r\n"));
        cleaned_line.erase(cleaned_line.find_last_not_of(" \t\r\n") + 1);

        spdlog::debug("[CharacterStringLineHandler] Cleaned line: {0} [{1}:{2}]", cleaned_line, __FILENAME__, __LINE__);
        return cleaned_line;
    }

    std::string CharacterStringLineHandler::RemoveLineComments(const std::string& line_to_be_processed) const {
        spdlog::trace("[CharacterStringLineHandler] RemoveLineComments() called [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Removing comments...");

        spdlog::debug("[CharacterStringLineHandler] Removing line comments for line: {0} [{1}:{2}]", line_to_be_processed, __FILENAME__, __LINE__);
        std::string cleaned_line = line_to_be_processed;
        size_t commentPos = cleaned_line.find("//");
        if (commentPos != std::string::npos) {
            cleaned_line = cleaned_line.substr(0, commentPos);
        }
        spdlog::debug("[CharacterStringLineHandler] ['//'] Comment removed from line: {0} [{1}:{2}]", cleaned_line, __FILENAME__, __LINE__);

        tools::PrintGreenOKMessage("Removing block comments...");
        spdlog::debug("[CharacterStringLineHandler] Removing block comments for line: {0} [{1}:{2}]", line_to_be_processed, __FILENAME__, __LINE__);
        commentPos = cleaned_line.find(";");
        if (commentPos != std::string::npos) {
            cleaned_line = cleaned_line.substr(0, commentPos);
        }
        spdlog::debug("[CharacterStringLineHandler] [';'] Comment removed from line: {0} [{1}:{2}]", cleaned_line, __FILENAME__, __LINE__);

        tools::PrintGreenOKMessage("Block comments removed.");
        spdlog::debug("[CharacterStringLineHandler] Block comments removed for line: {0} [{1}:{2}]", line_to_be_processed, __FILENAME__, __LINE__);
        return cleaned_line;
    }

    bool CharacterStringLineHandler::IsNumber(const std::string& string_number) const {
        spdlog::trace("[CharacterStringLineHandler] IsNumber() called [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Checking if string is a number...");

        spdlog::trace("[CharacterStringLineHandler] Checking if string: {0} is a number [{1}:{2}]", string_number, __FILENAME__, __LINE__);
        if (string_number.empty()) {
            spdlog::debug("[CharacterStringLineHandler] String is empty, not a number [{0}:{1}]", __FILENAME__, __LINE__);
            tools::PrintRedErrorMessage("String is empty, not a number.");
            return false;
        }

        spdlog::debug("[CharacterStringLineHandler] Checking if immediate value... [{0}:{1}]", string_number, __FILENAME__, __LINE__);
        size_t start = 0;
        if (string_number[0] == '#') {
            spdlog::debug("[CharacterStringLineHandler] Immediate value detected [{0}:{1}]", __FILENAME__, __LINE__);
            start = 1;  // Immediate value
        }

        spdlog::debug("[CharacterStringLineHandler] Checking if hex value... [{0}:{1}]", string_number, __FILENAME__, __LINE__);
        if (string_number.length() > start + 2 && string_number.substr(start, 2) == "0x") {
            for (size_t i = start + 2; i < string_number.length(); i++) {
                if (!std::isxdigit(string_number[i])) {
                    spdlog::debug("[CharacterStringLineHandler] Invalid hex digit found: {0} [{1}:{2}]", string_number[i], __FILENAME__, __LINE__);
                    tools::PrintRedErrorMessage("Invalid hex digit found.");
                    return false;
                }
                spdlog::trace("[CharacterStringLineHandler] Valid hex digit: {0} [{1}:{2}]", string_number[i], __FILENAME__, __LINE__);
            }
            spdlog::debug("[CharacterStringLineHandler] Hex value is valid [{0}:{1}]", __FILENAME__, __LINE__);
            tools::PrintGreenOKMessage("Hex value is valid.");
            return true;
        }

        spdlog::debug("[CharacterStringLineHandler] Checking if decimal value... [{0}:{1}]", __FILENAME__, __LINE__);
        for (size_t i = start; i < string_number.length(); i++) {
            if (!std::isdigit(string_number[i])) {
                spdlog::debug("[CharacterStringLineHandler] Invalid decimal digit found: {0} [{1}:{2}]", string_number[i], __FILENAME__, __LINE__);
                tools::PrintRedErrorMessage("Invalid decimal digit found.");
                return false;
            }
        }
        spdlog::debug("[CharacterStringLineHandler] Decimal value is valid [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Decimal value is valid.");
        return true;
    }

    uint8_t CharacterStringLineHandler::ConvertStringToNumber(const std::string& string_number) const {
        spdlog::trace("[CharacterStringLineHandler] ConvertStringToNumber() called [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Converting string to number...");

        spdlog::debug("[CharacterStringLineHandler] Converting string: {0} to number [{1}:{2}]", string_number, __FILENAME__, __LINE__);
        std::string num = string_number;
        spdlog::debug("[CharacterStringLineHandler] Initial string: {0} [{1}:{2}]", num, __FILENAME__, __LINE__);
        if (num[0] == '#') {
            spdlog::debug("[CharacterStringLineHandler] Removing '#' from string [{0}:{1}]", __FILENAME__, __LINE__);
            num = num.substr(1);  // Remove '#' for immediate values
            spdlog::debug("[CharacterStringLineHandler] String after removing '#': {0} [{1}:{2}]", num, __FILENAME__, __LINE__);
        }

        spdlog::debug("[CharacterStringLineHandler] Checking if hex format... [{0}:{1}]", num, __FILENAME__, __LINE__);
        if (num.length() > 2 && num.substr(0, 2) == "0x") {
            spdlog::debug("[CharacterStringLineHandler] Hex format detected, converting... [{0}:{1}]", __FILENAME__, __LINE__);
            tools::PrintGreenOKMessage("Converting hex string to number...");
            return static_cast<uint8_t>(std::stoul(num, nullptr, 16));
        }
        spdlog::debug("[CharacterStringLineHandler] Decimal format detected, converting... [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Converting decimal string to number...");

        spdlog::debug("[CharacterStringLineHandler] String number after processing: {0} [{1}:{2}]", num, __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("String converted to number successfully.");
        return static_cast<uint8_t>(std::stoul(num));
    }

    std::vector<std::string> CharacterStringLineHandler::TokenizeLine(const std::string& line_to_be_processed) const {
        spdlog::trace("[CharacterStringLineHandler] TokenizeLine() called [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Tokenizing line...");

        spdlog::debug("[CharacterStringLineHandler] Tokenizing line: {0} [{1}:{2}]", line_to_be_processed, __FILENAME__, __LINE__);
        std::vector<std::string> tokens;
        std::istringstream iss(line_to_be_processed);
        std::string token;

        spdlog::debug("[CharacterStringLineHandler] Splitting line into tokens... [{0}:{1}]", __FILENAME__, __LINE__);
        while (iss >> token) {
            if (!token.empty() && token.back() == ',') {
                spdlog::debug("[CharacterStringLineHandler] Removing trailing comma ',' from token: {0} [{1}:{2}]", token, __FILENAME__, __LINE__);
                token.pop_back();
                spdlog::debug("[CharacterStringLineHandler] Token after removing comma: {0} [{1}:{2}]", token, __FILENAME__, __LINE__);
            }
            if (!token.empty()) {
                spdlog::debug("[CharacterStringLineHandler] Adding token: {0} [{1}:{2}]", token, __FILENAME__, __LINE__);
                tokens.push_back(token);
            }
        }

        spdlog::debug("[CharacterStringLineHandler] Tokenization complete. Total tokens: {0} [{1}:{2}]", tokens.size(), __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Line tokenized successfully.");
        return tokens;
    }

}  // namespace assembly_engine