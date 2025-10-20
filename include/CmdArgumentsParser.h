#pragma once

#include <memory>

#include "ArgumentsParserLogic.h"
#include "CmdArguments.h"

namespace cmd {
    class CmdArgumentsParser {
      public:
        CmdArgumentsParser(int argc, char* argv[]);
        bool Parse(std::shared_ptr<CmdArguments> cmd_arguments);

      protected:
        CmdArgumentsParser(int argc, char* argv[], std::unique_ptr<IArgumentsParserLogic> argument_parser_logic);

      private:
        int m_argc;
        char** m_argv;
        std::unique_ptr<IArgumentsParserLogic> argument_parser_logic_;
    };
}  // namespace cmd