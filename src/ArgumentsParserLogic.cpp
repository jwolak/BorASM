#include "ArgumentsParserLogic.h"

#include <fmt/format.h>

#include <iostream>

#include "Tools.h"
#include "EquinoxLogger.hpp"
#include "version.h"

namespace cmd {
    namespace {
        constexpr const char* kInputFileExtension = ".asm";
        constexpr const char* kOutputFileExtension = ".hex";
        constexpr std::size_t kInputFileExtensionLength = 4;
    }  // namespace

    ArgumentsParserLogic::ArgumentsParserLogic() {}

    void ArgumentsParserLogic::PrintHelp() const {
        LOG_DEBUG("[ArgumentsParserLogic] Printing help information");

        fmt::print(
            "BorASM - Assembler CLI\n"
            "Version: {}\n"
            "\nUsage:\n"
            "  BorASM [options]\n"
            "\nOptions:\n"
            "  -h, --help                 Show this help message\n"
            "  -v, --version              Show version information\n"
            "  -D, --debug                Enable debug mode\n"
            "  -i, --input <file>         Input assembly source file\n"
            "  -o, --output <file>        Output file name\n"
            "  -l, --list                 List all available instructions\n"
            "\nExample:\n"
            "  BorASM -i code.asm -o code.bin\n",
            BorASM::Version::GetVersionString());
    }

    void ArgumentsParserLogic::PrintVersionInfo() const {
        LOG_DEBUG("[ArgumentsParserLogic] Printing version information");

        fmt::print("Version: {}\n", BorASM::Version::GetVersionString());
        fmt::print("Full Version: {}\n", BorASM::Version::GetFullVersionString());
        fmt::print("Complete Version: {}\n", BorASM::Version::GetCompleteVersionInfo());
        fmt::print("Build Type: {}\n", BorASM::Version::BUILD_TYPE);
        fmt::print("Is version at least 0.1.0 ? {}\n", BorASM::Version::IsVersionAtLeast(0, 1, 0) ? "[Yes]" : "[No]");
    }

    bool ArgumentsParserLogic::EnableDebugMode() const {
        tools::PrintYellowWarningMessage("Enabling debug mode...");
        try {
            CHANGE_LOG_LEVEL(equinox::logger::LogLevel::Debug);
        } catch (const std::exception& e) {
            std::cerr << "Error enabling debug mode: " << e.what() << std::endl;
            tools::PrintRedErrorMessage("Failed to enable debug mode.");
            return false;
        }

        LOG_DEBUG(std::string("[ArgumentsParserLogic] Debug mode activated [") + __FILENAME__ + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Debug mode enabled successfully.");
        return true;
    }

    std::optional<std::string> ArgumentsParserLogic::GetInputFileName(const char* optarg) const {
        LOG_DEBUG("[ArgumentsParserLogic] Retrieving input file name");

        if (optarg) {
            tools::PrintWithGreenMarker("Input file", "Input file name provided: " + std::string(optarg));
            return std::string(optarg);
        }
        std::cerr << "[ERROR] Input file name argument is null" << std::endl;
        tools::PrintRedErrorMessage("Input file name argument is null.");
        return std::nullopt;
    }

    std::optional<std::string> ArgumentsParserLogic::GetOutputFileName(const char* optarg) const {
        LOG_DEBUG("[ArgumentsParserLogic] Retrieving output file name");

        if (optarg) {
            tools::PrintWithGreenMarker("Output file", "Output file name provided: " + std::string(optarg));
            return std::string(optarg);
        }
        std::cerr << "[ERROR] Output file name argument is null" << std::endl;
        tools::PrintRedErrorMessage("Output file name argument is null.");
        return std::nullopt;
    }

    void ArgumentsParserLogic::ListAvailableInstructions() const {
        fmt::print("Available instructions:\n");
        fmt::print("\nArithmetic instructions (0x00-0x07):\n");
        fmt::print("  ADD   0x00   Add:        ADD reg, reg/imm\n");
        fmt::print("  SUB   0x01   Subtract:   SUB reg, reg/imm\n");
        fmt::print("  AND   0x02   And:        AND reg, reg/imm\n");
        fmt::print("  OR    0x03   Or:         OR reg, reg/imm\n");
        fmt::print("  XOR   0x04   Xor:        XOR reg, reg/imm\n");
        fmt::print("  MOV   0x05   Move:       MOV reg, reg/imm\n");
        fmt::print("  SHL   0x06   Shift left: SHL reg, reg/imm\n");
        fmt::print("  SHR   0x07   Shift right:SHR reg, reg/imm\n");

        fmt::print("\nJump instructions (0x08-0x0E):\n");
        fmt::print("  JMP   0x08   Unconditional jump\n");
        fmt::print("  JZ    0x09   Jump if zero\n");
        fmt::print("  JNZ   0x0A   Jump if not zero\n");
        fmt::print("  JC    0x0B   Jump if carry\n");
        fmt::print("  JNC   0x0C   Jump if not carry\n");
        fmt::print("  JN    0x0D   Jump if negative\n");
        fmt::print("  JNN   0x0E   Jump if not negative\n");

        fmt::print("\nSpecial instructions:\n");
        fmt::print("  CMP   0x0F   Compare\n");
        fmt::print("  HALT  0xFF   Halt execution\n");
    }

    std::string ArgumentsParserLogic::SetInputFileAsOutputFileName(const std::string& input_file_name) const {
        LOG_DEBUG("[ArgumentsParserLogic] Set input file name:" + input_file_name + " as output file name");

        std::string output_name = input_file_name;
        if (output_name.size() >= kInputFileExtensionLength && output_name.substr(output_name.size() - kInputFileExtensionLength) == kInputFileExtension) {
            output_name = output_name.substr(0, output_name.size() - kInputFileExtensionLength);
        }
        output_name += kOutputFileExtension;
        LOG_DEBUG("[ArgumentsParserLogic] Output file name set to: " + output_name);

        tools::PrintWithGreenMarker("Output file", "Output file name set to: " + output_name);
        return output_name;
    }

    bool ArgumentsParserLogic::CheckOutputFileNameIsNotSameAsInputFileName(const std::string& input_file_name, const std::string& output_file_name) const {
        LOG_DEBUG("[ArgumentsParserLogic] Checking if output file name is not the same as input file name");

        if (input_file_name == output_file_name) {
            LOG_DEBUG("[ArgumentsParserLogic] Output file name cannot be the same as input file name");
            return false;
        }

        LOG_DEBUG("[ArgumentsParserLogic] Output file name is different from input file name");
        return true;
    }

}  // namespace cmd