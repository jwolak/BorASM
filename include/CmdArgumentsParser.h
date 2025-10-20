#pragma once

#include <memory>

#include "ArgumentsParserLogic.h"

namespace cmd {
    class CmdArgumentsParser {
      public:
        CmdArgumentsParser(int argc, char* argv[]);
        ~CmdArgumentsParser();
        bool Parse();

      private:
        int m_argc;
        char** m_argv;
        std::unique_ptr<ArgumentsParserLogic> argument_parser_logic_;
    };
}  // namespace cmd