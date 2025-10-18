#include "CmdArgumentsParser.h"

#include <getopt.h>

#include <iostream>

#include "version.h"

namespace cmd {
    CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[])
        : m_argc(argc), m_argv(argv), argument_parser_logic_(std::make_unique<ArgumentsParserLogic>()) {}

    CmdArgumentsParser::~CmdArgumentsParser() {}

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
                    argument_parser_logic_->PrintHelp();
                    break;
                case 'v':
                    argument_parser_logic_->PrintVersionInfo();
                    break;
                case 'd':
                    argument_parser_logic_->EnableDebugMode();
                    break;
                case 'i':
                    argument_parser_logic_->GetInputFileName(optarg);
                    break;
                case 'o':
                    argument_parser_logic_->GetOutputFileName(optarg);
                    break;
                case 'l':
                    argument_parser_logic_->ListAvailableInstructions();
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