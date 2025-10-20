#include <gtest/gtest.h>

#include <memory>

#include "ArgumentsParserLogicMock.h"
#include "CmdArgumentsParser.h"

namespace cmd_arguments_parser_test {

    class CmdArgumentsParserWithInjectedLogic : public cmd::CmdArgumentsParser {
      public:
        CmdArgumentsParserWithInjectedLogic(int argc, char* argv[], std::unique_ptr<cmd::IArgumentsParserLogic> argument_parser_logic)
            : cmd::CmdArgumentsParser(argc, argv, std::move(argument_parser_logic)) {}
    };

    class CmdArgumentsParserTest : public ::testing::Test {
      public:
        CmdArgumentsParserTest()
            : argument_parser_logic_mock{new mocks::ArgumentsParserLogicMock},
              cmd_arguments_parser_with_injected_logic(1, nullptr, std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock)) {}
        void SetUp() override {}

        void TearDown() override {}

        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock;
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic;
    };
}  // namespace cmd_arguments_parser_test