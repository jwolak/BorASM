#include "CmdArgumentsParser.h"

#include <getopt.h>

#include <iostream>

#include "version.h"

namespace cmd {
    CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[])
        : m_argc(argc), m_argv(argv), argument_parser_logic_(std::make_unique<ArgumentsParserLogic>()) {}

    CmdArgumentsParser::~CmdArgumentsParser() {}

    bool CmdArgumentsParser::Parse(std::shared_ptr<CmdArguments> cmd_arguments) {
        static struct option longopts[] = {
            {"help", no_argument, NULL, 'h'},        {"version", no_argument, NULL, 'v'},      {"debug", no_argument, NULL, 'd'},
            {"input", required_argument, NULL, 'i'}, {"output", required_argument, NULL, 'o'}, {"list", no_argument, NULL, 'l'},
        };

        if (m_argc < 2) {
            std::cerr << "\n[ERROR] No arguments provided!\n\n";
            argument_parser_logic_->PrintHelp();
            return false;
        }

        int opt;
        int longindex;
        bool inputSet = false;
        bool outputSet = false;
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
                    cmd_arguments->input_file_path = argument_parser_logic_->GetInputFileName(optarg);
                    if (cmd_arguments->input_file_path == std::nullopt) {
                        std::cerr << "Invalid input file name provided." << std::endl;
                        return false;
                    }
                    inputSet = true;
                    break;
                case 'o':
                    cmd_arguments->output_file_path = argument_parser_logic_->GetOutputFileName(optarg);
                    if (cmd_arguments->output_file_path == std::nullopt) {
                        std::cerr << "Invalid output file name provided." << std::endl;
                        return false;
                    }
                    outputSet = true;
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

        if (!inputSet || !outputSet) {
            std::cerr << "\n[ERROR] Both -i <input_file> and -o <output_file> options are required.\n\n";
            argument_parser_logic_->PrintHelp();
            return false;
        }

        return true;
    }
}  // namespace cmd