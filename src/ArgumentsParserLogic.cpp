#include "ArgumentsParserLogic.h"

#include <spdlog/spdlog.h>

#include <iostream>

#include "version.h"

namespace cmd {
    ArgumentsParserLogic::ArgumentsParserLogic() {}

    void ArgumentsParserLogic::PrintHelp() const {
        std::cout << "BorASM - Assembler CLI\n"
                  << "Version: " << BorASM::Version::GetVersionString() << "\n"
                  << "\nUsage:\n"
                  << "  BorASM [options]\n"
                  << "\nOptions:\n"
                  << "  -h, --help                 Show this help message\n"
                  << "  -v, --version              Show version information\n"
                  << "  -D, --debug                Enable debug mode\n"
                  << "  -i, --input <file>         Input assembly source file\n"
                  << "  -o, --output <file>        Output file name\n"
                  << "  -l, --list                 List all available instructions\n"
                  << "\nExample:\n"
                  << "  BorASM -i code.asm -o code.bin\n";
    }

    void ArgumentsParserLogic::PrintVersionInfo() const {
        std::cout << BorASM::Version::GetVersionString() << std::endl;
        std::cout << BorASM::Version::GetFullVersionString() << std::endl;
        std::cout << BorASM::Version::GetCompleteVersionInfo() << std::endl;
        std::cout << BorASM::Version::BUILD_TYPE << std::endl;
        std::cout << "Is version at least 0.1.0? " << (BorASM::Version::IsVersionAtLeast(0, 1, 0) ? "true" : "false") << std::endl;
    }

    bool ArgumentsParserLogic::EnableDebugMode() const {
        std::cout << "Debug mode enabled" << std::endl;
        try {
            spdlog::set_level(spdlog::level::debug);
        } catch (const std::exception& e) {
            std::cerr << "Error enabling debug mode: " << e.what() << std::endl;
            return false;
        }
        return true;
    }

    std::optional<std::string> ArgumentsParserLogic::GetInputFileName(const char* optarg) const {
        if (optarg) {
            std::cout << "Input file name: " << optarg << std::endl;
            return std::string(optarg);
        }
        std::cerr << "[ERROR] Input file name argument is null" << std::endl;
        return std::nullopt;
    }

    std::optional<std::string> ArgumentsParserLogic::GetOutputFileName(const char* optarg) const {
        if (optarg) {
            std::cout << "Output file name: " << optarg << std::endl;
            return std::string(optarg);
        }
        std::cerr << "[ERROR] Output file name argument is null" << std::endl;
        return std::nullopt;
    }

    void ArgumentsParserLogic::ListAvailableInstructions() const {
        std::cout << "Available instructions:\n";
        std::cout << "\nArithmetic instructions (0x00-0x07):\n";
        std::cout << "  ADD   0x00   Add:        ADD reg, reg/imm\n";
        std::cout << "  SUB   0x01   Subtract:   SUB reg, reg/imm\n";
        std::cout << "  AND   0x02   And:        AND reg, reg/imm\n";
        std::cout << "  OR    0x03   Or:         OR reg, reg/imm\n";
        std::cout << "  XOR   0x04   Xor:        XOR reg, reg/imm\n";
        std::cout << "  MOV   0x05   Move:       MOV reg, reg/imm\n";
        std::cout << "  SHL   0x06   Shift left: SHL reg, reg/imm\n";
        std::cout << "  SHR   0x07   Shift right:SHR reg, reg/imm\n";

        std::cout << "\nJump instructions (0x08-0x0E):\n";
        std::cout << "  JMP   0x08   Unconditional jump\n";
        std::cout << "  JZ    0x09   Jump if zero\n";
        std::cout << "  JNZ   0x0A   Jump if not zero\n";
        std::cout << "  JC    0x0B   Jump if carry\n";
        std::cout << "  JNC   0x0C   Jump if not carry\n";
        std::cout << "  JN    0x0D   Jump if negative\n";
        std::cout << "  JNN   0x0E   Jump if not negative\n";

        std::cout << "\nSpecial instructions:\n";
        std::cout << "  CMP   0x0F   Compare\n";
        std::cout << "  HALT  0xFF   Halt execution\n";
    }
}  // namespace cmd