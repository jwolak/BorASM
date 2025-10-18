#include "ArgumentsParserLogic.h"

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
        return true;
    }

    std::optional<std::string> ArgumentsParserLogic::GetInputFileName(const char* optarg) const {
        if (optarg) {
            return std::string(optarg);
        }
        return std::nullopt;
    }

    std::optional<std::string> ArgumentsParserLogic::GetOutputFileName(const char* optarg) const {
        if (optarg) {
            return std::string(optarg);
        }
        return std::nullopt;
    }

    void ArgumentsParserLogic::ListAvailableInstructions() const {
        // Implementation to list instructions
    }
}  // namespace cmd