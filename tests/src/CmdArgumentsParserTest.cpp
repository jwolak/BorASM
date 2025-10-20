#include <gtest/gtest.h>

#include <memory>

#include "ArgumentsParserLogicMock.h"
#include "CmdArgumentsParser.h"

extern int optind;

namespace cmd_arguments_parser_test {

    using namespace testing;

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

        void SetUp() override { optind = 0; }

        void TearDown() override {}

        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock;
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic;
    };

    TEST_F(CmdArgumentsParserTest, Arguments_Number_Less_Than_Two_Should_Return_False) {
        testing::internal::CaptureStderr();
        auto cmd_arguments = std::make_shared<cmd::CmdArguments>();
        EXPECT_CALL(*argument_parser_logic_mock, PrintHelp()).Times(1);
        bool result = cmd_arguments_parser_with_injected_logic.Parse(cmd_arguments);
        std::string output = testing::internal::GetCapturedStderr();

        EXPECT_FALSE(result);
        EXPECT_NE(output.find("No arguments provided!"), std::string::npos);
    }

    TEST_F(CmdArgumentsParserTest, Print_Help_Option_Should_Invoke_PrintHelp_And_Return_True) {
        auto cmd_arguments = std::make_shared<cmd::CmdArguments>();
        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock = new mocks::ArgumentsParserLogicMock;
        const char* argv[] = {"program", "--help"};
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic =
            CmdArgumentsParserWithInjectedLogic(2, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, PrintHelp()).Times(1);
        bool result = cmd_arguments_parser_with_injected_logic.Parse(cmd_arguments);

        EXPECT_TRUE(result);
    }

    TEST_F(CmdArgumentsParserTest, Print_Help_Option_Should_Invoke_PrintVersionInfo_And_Return_True) {
        auto cmd_arguments = std::make_shared<cmd::CmdArguments>();
        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock = new mocks::ArgumentsParserLogicMock;
        const char* argv[] = {"program", "--version"};
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(2, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, PrintVersionInfo()).Times(1);
        bool result = cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments);

        EXPECT_TRUE(result);
    }

    TEST_F(CmdArgumentsParserTest, Enable_Debug_Option_Should_Invoke_EnableDebugMode_And_Return_True) {
        auto cmd_arguments = std::make_shared<cmd::CmdArguments>();
        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock = new mocks::ArgumentsParserLogicMock;
        const char* argv[] = {"program", "--debug", "--input", "input.asm", "--output", "output.bin"};
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(6, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, EnableDebugMode()).Times(1);
        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName("input.asm")).Times(1).WillOnce(testing::Return("input.asm"));
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName("output.bin")).Times(1).WillOnce(testing::Return("output.bin"));
        bool result = cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments);

        EXPECT_TRUE(result);
    }

    TEST_F(CmdArgumentsParserTest, Provide_Valid_Input_And_Output_Files_And_Return_True) {
        auto cmd_arguments = std::make_shared<cmd::CmdArguments>();
        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock = new mocks::ArgumentsParserLogicMock;
        const char* argv[] = {"program", "--input", "input.asm", "--output", "output.bin"};
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(5, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName("input.asm")).Times(1).WillOnce(testing::Return("input.asm"));
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName("output.bin")).Times(1).WillOnce(testing::Return("output.bin"));
        bool result = cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments);

        EXPECT_TRUE(result);
    }

    TEST_F(CmdArgumentsParserTest, Provide_Only_Input_File_And_Return_False) {
        auto cmd_arguments = std::make_shared<cmd::CmdArguments>();
        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock = new mocks::ArgumentsParserLogicMock;
        const char* argv[] = {"program", "--input", "input.asm"};
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(5, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName("input.asm")).Times(1).WillOnce(testing::Return("input.asm"));
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName(_)).Times(0);
        bool result = cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments);

        EXPECT_FALSE(result);
    }

    TEST_F(CmdArgumentsParserTest, Provide_Only_Output_File_And_Return_False) {
        auto cmd_arguments = std::make_shared<cmd::CmdArguments>();
        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock = new mocks::ArgumentsParserLogicMock;
        const char* argv[] = {"program", "--output", "output.bin"};
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(5, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName(_)).Times(0);
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName("output.bin")).Times(1).WillOnce(testing::Return("output.bin"));
        bool result = cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments);

        EXPECT_FALSE(result);
    }

    TEST_F(CmdArgumentsParserTest, Print_List_Option_Should_Invoke_ListAvailableInstructions_And_Return_True) {
        auto cmd_arguments = std::make_shared<cmd::CmdArguments>();
        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock = new mocks::ArgumentsParserLogicMock;
        const char* argv[] = {"program", "--list"};
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic =
            CmdArgumentsParserWithInjectedLogic(2, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, ListAvailableInstructions()).Times(1);
        bool result = cmd_arguments_parser_with_injected_logic.Parse(cmd_arguments);

        EXPECT_TRUE(result);
    }

}  // namespace cmd_arguments_parser_test