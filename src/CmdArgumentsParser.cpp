#include "CmdArgumentsParser.h"

#include <iostream>

#include "version.h"

namespace cmd {
    namespace {
        const char kHelpMenuPrint[] =
            "\t-h [--help]\n"
            "\t-v [--version]\n\n"
            "\t-D [--Debug]                Enable debug mode\n"
            "\t-i [--input] <input_file>   Input assembly source file\n"
            "\t-o [--output] <output_file> Output file name\n"
            "\t-l [--list]                 List all available instructions\n";
    }

    CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[]) : m_argc(argc), m_argv(argv) {}

    CmdArgumentsParser::~CmdArgumentsParser() {}

    void CmdArgumentsParser::PrintHelp() const { std::cout << kHelpMenuPrint << std::endl; }

    void CmdArgumentsParser::PrintVersionInfo() const {
        std::cout << BorASM::Version::GetVersionString();        // "0.1.0"
        std::cout << BorASM::Version::GetFullVersionString();    // "0.1.0.289"
        std::cout << BorASM::Version::GetCompleteVersionInfo();  // "0.1.0.289 (develop@67e5d1f, built 2025-10-16 18:16:00)"
        std::cout << BorASM::Version::BUILD_TYPE;                // "Debug" lub "Release"
        std::cout << "Is version at least 0.1.0? " << (BorASM::Version::IsVersionAtLeast(0, 1, 0) ? "true" : "false") << std::endl;  // true
    }

    void CmdArgumentsParser::Parse() {
        // Parse command-line arguments
    }
}  // namespace cmd