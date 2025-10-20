#pragma once

#include <memory>

#include "ArgumentsParserLogic.h"
#include "CmdArguments.h"

namespace cmd {
    class CmdArgumentsParser {
      public:
        CmdArgumentsParser(int argc, char* argv[]);
        ~CmdArgumentsParser();
        bool Parse(std::shared_ptr<CmdArguments> cmd_arguments);

      private:
        int m_argc;
        char** m_argv;
        std::unique_ptr<ArgumentsParserLogic> argument_parser_logic_;
    };
}  // namespace cmd