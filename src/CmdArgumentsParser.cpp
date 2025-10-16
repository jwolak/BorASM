#include "CmdArgumentsParser.h"

#include <getopt.h>

#include <iostream>

#include "version.h"

namespace cmd {
    CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[]) : m_argc(argc), m_argv(argv) {}

    CmdArgumentsParser::~CmdArgumentsParser() {}

    void CmdArgumentsParser::PrintHelp() const {
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

    void CmdArgumentsParser::PrintVersionInfo() const {
        std::cout << BorASM::Version::GetVersionString();        // "0.1.0"
        std::cout << BorASM::Version::GetFullVersionString();    // "0.1.0.289"
        std::cout << BorASM::Version::GetCompleteVersionInfo();  // "0.1.0.289 (develop@67e5d1f, built 2025-10-16 18:16:00)"
        std::cout << BorASM::Version::BUILD_TYPE;                // "Debug" lub "Release"
        std::cout << "Is version at least 0.1.0? " << (BorASM::Version::IsVersionAtLeast(0, 1, 0) ? "true" : "false") << std::endl;  // true
    }

    void CmdArgumentsParser::Parse() {
        static struct option longopts[] = {
            {"help", no_argument, NULL, 'h'},        {"version", no_argument, NULL, 'v'},      {"debug", no_argument, NULL, 'd'},
            {"input", required_argument, NULL, 'i'}, {"output", required_argument, NULL, 'o'}, {"list", no_argument, NULL, 'l'},
        };

        int opt;
        int longindex;
        while ((opt = getopt_long(m_argc, m_argv, "hvdi:o:l", longopts, &longindex)) != -1) {
            switch (opt) {
                case 'h':
                    PrintHelp();
                    break;
                case 'v':
                    PrintVersionInfo();
                    break;
                case 'd':
                    std::cout << "Debug mode enabled" << std::endl;
                    break;
                case 'i':
                    std::cout << "Input file: " << optarg << std::endl;
                    break;
                case 'o':
                    std::cout << "Output file: " << optarg << std::endl;
                    break;
                case 'l':
                    std::cout << "Listing all available instructions..." << std::endl;
                    break;
                case '?':
                    std::cerr << "Unknown option or missing argument. Use -h or --help for help." << std::endl;
                    break;
                default:
                    break;
            }
        }
    }
}  // namespace cmd