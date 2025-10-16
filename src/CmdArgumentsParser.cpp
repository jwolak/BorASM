#include "CmdArgumentsParser.h"

#include "version.h"

namespace cmd {
CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[]) : m_argc(argc), m_argv(argv) {}

CmdArgumentsParser::~CmdArgumentsParser() {}

void CmdArgumentsParser::PrintHelp() const {
    // Print help information
}

void CmdArgumentsParser::PrintVersion() const {
    // Print version information
    // Przykłady użycia:
    BorASM::Version::GetVersionString();         // "0.1.0"
    BorASM::Version::GetFullVersionString();     // "0.1.0.289"
    BorASM::Version::GetCompleteVersionInfo();   // "0.1.0.289 (develop@67e5d1f, built 2025-10-16 18:16:00)"
    BorASM::Version::BUILD_TYPE;                 // "Debug" lub "Release"
    BorASM::Version::IsVersionAtLeast(0, 1, 0);  // true
}

void CmdArgumentsParser::Parse() {
    // Parse command-line arguments
}
}  // namespace cmd