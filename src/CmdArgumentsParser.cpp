#include "CmdArgumentsParser.h"

#include <getopt.h>

#include <iostream>
#include <memory>

#include "Tools.h"
#include "version.h"

namespace cmd {
    CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[])
        : m_argc(argc), m_argv(argv), argument_parser_logic_(std::make_unique<ArgumentsParserLogic>()) {}

    /* For tests purpose only */
    CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[], std::unique_ptr<IArgumentsParserLogic> argument_parser_logic)
        : m_argc(argc), m_argv(argv), argument_parser_logic_(std::move(argument_parser_logic)) {}

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
        bool input_set = false;
        bool output_set = false;
        bool help_of_info_shown = false;
        while ((opt = getopt_long(m_argc, m_argv, "hvdi:o:l", longopts, &longindex)) != -1) {
            switch (opt) {
                case 'h':
                    argument_parser_logic_->PrintHelp();
                    help_of_info_shown = true;
                    return false;
                case 'v':
                    argument_parser_logic_->PrintVersionInfo();
                    help_of_info_shown = true;
                    return false;
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
                    input_set = true;
                    break;
                case 'o':
                    cmd_arguments->output_file_path = argument_parser_logic_->GetOutputFileName(optarg);
                    if (cmd_arguments->output_file_path == std::nullopt) {
                        tools::PrintRedErrorMessage("Invalid output file name provided.\n");
                        return false;
                    }
                    tools::PrintGreenOKMessage("Output file name set to: " + *cmd_arguments->output_file_path);
                    output_set = true;
                    break;
                case 'l':
                    argument_parser_logic_->ListAvailableInstructions();
                    help_of_info_shown = true;
                    return false;
                case '?':
                    tools::PrintRedErrorMessage("Unknown option or missing argument. Use -h or --help for help.\n");
                    break;
                default:
                    break;
            }
        }

        if ((!input_set || !output_set) && !help_of_info_shown) {
            tools::PrintRedErrorMessage("Both -i <input_file> and -o <output_file> options are required.\n");
            return false;
        }

        tools::PrintGreenOKMessage("Command line arguments parsed successfully.");
        return true;
    }
}  // namespace cmd