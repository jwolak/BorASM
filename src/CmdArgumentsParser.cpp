#include "CmdArgumentsParser.h"

#include <getopt.h>

#include <iostream>

#include "Tools.h"
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
            tools::PrintRedErrorMessage("No arguments provided!\n\n");
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
                    tools::PrintYellowWarningMessage("Debug mode enabled.");
                    break;
                case 'i':
                    cmd_arguments->input_file_path = argument_parser_logic_->GetInputFileName(optarg);
                    if (cmd_arguments->input_file_path == std::nullopt) {
                        tools::PrintRedErrorMessage("Invalid input file name provided.\n");
                        return false;
                    }
                    tools::PrintGreenOKMessage("Input file name set to: " + *cmd_arguments->input_file_path);
                    inputSet = true;
                    break;
                case 'o':
                    cmd_arguments->output_file_path = argument_parser_logic_->GetOutputFileName(optarg);
                    if (cmd_arguments->output_file_path == std::nullopt) {
                        tools::PrintRedErrorMessage("Invalid output file name provided.\n");
                        return false;
                    }
                    tools::PrintGreenOKMessage("Output file name set to: " + *cmd_arguments->output_file_path);
                    outputSet = true;
                    break;
                case 'l':
                    argument_parser_logic_->ListAvailableInstructions();
                    break;
                case '?':
                    tools::PrintRedErrorMessage("Unknown option or missing argument. Use -h or --help for help.\n");
                    break;
                default:
                    break;
            }
        }

        if (!inputSet || !outputSet) {
            tools::PrintRedErrorMessage("Both -i <input_file> and -o <output_file> options are required.\n");
            argument_parser_logic_->PrintHelp();
            return false;
        }

        tools::PrintGreenOKMessage("Command line arguments parsed successfully.");
        return true;
    }
}  // namespace cmd