#include <gtest/gtest.h>

#include <memory>

#include "ArgumentsParserLogicMock.h"
#include "CmdArgumentsParser.h"

extern int optind;

namespace cmd_arguments_parser_test {

    using namespace testing;

    namespace {
        constexpr const char* kInputFileName = "input.asm";
        constexpr const char* kOutputFileName = "output.bin";
    }  // namespace

    class CmdArgumentsParserWithInjectedLogic : public cmd::CmdArgumentsParser {
      public:
        CmdArgumentsParserWithInjectedLogic(int argc, char* argv[], std::unique_ptr<cmd::IArgumentsParserLogic> argument_parser_logic)
            : cmd::CmdArgumentsParser(argc, argv, std::move(argument_parser_logic)) {}
    };

    class CmdArgumentsParserTest : public ::testing::Test {
      public:
        CmdArgumentsParserTest() : cmd_arguments{std::make_shared<cmd::CmdArguments>()}, argument_parser_logic_mock{new mocks::ArgumentsParserLogicMock} {}
        void SetUp() override { optind = 0; }
        void TearDown() override {}

        std::shared_ptr<cmd::CmdArguments> cmd_arguments;
        mocks::ArgumentsParserLogicMock* argument_parser_logic_mock;
    };

    TEST_F(CmdArgumentsParserTest, Arguments_Number_Less_Than_Two_Should_Return_False) {
        testing::internal::CaptureStderr();
        const char* argv[] = {"program"};
        EXPECT_CALL(*argument_parser_logic_mock, PrintHelp()).Times(1);
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));
        bool result = cmd_arguments_parser_with_injected_logic.Parse(cmd_arguments);
        std::string output = testing::internal::GetCapturedStderr();

        EXPECT_FALSE(result);
        EXPECT_NE(output.find("No arguments provided!"), std::string::npos);
    }

    TEST_F(CmdArgumentsParserTest, Print_Help_Option_Should_Invoke_PrintHelp_And_Return_False) {
        const char* argv[] = {"program", "--help"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));
        EXPECT_CALL(*argument_parser_logic_mock, PrintHelp()).Times(1);

        EXPECT_FALSE(cmd_arguments_parser_with_injected_logic.Parse(cmd_arguments));
    }

    TEST_F(CmdArgumentsParserTest, Print_Version_Option_Should_Invoke_PrintVersionInfo_And_Return_False) {
        const char* argv[] = {"program", "--version"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));
        EXPECT_CALL(*argument_parser_logic_mock, PrintVersionInfo()).Times(1);

        EXPECT_FALSE(cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments));
    }

    TEST_F(CmdArgumentsParserTest, Enable_Debug_Option_Should_Invoke_EnableDebugMode_And_Return_True) {
        const char* argv[] = {"program", "--debug", "--input", kInputFileName, "--output", kOutputFileName};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, EnableDebugMode()).Times(1);
        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName(kInputFileName)).Times(1).WillOnce(testing::Return(kInputFileName));
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName(kOutputFileName)).Times(1).WillOnce(testing::Return(kOutputFileName));

        EXPECT_TRUE(cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments));
    }

    TEST_F(CmdArgumentsParserTest, Provide_Valid_Input_And_Output_Files_And_Return_True) {
        const char* argv[] = {"program", "--input", kInputFileName, "--output", kOutputFileName};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName(kInputFileName)).Times(1).WillOnce(testing::Return(kInputFileName));
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName(kOutputFileName)).Times(1).WillOnce(testing::Return(kOutputFileName));

        EXPECT_TRUE(cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments));
    }

    TEST_F(CmdArgumentsParserTest, Provide_Only_Input_File_And_Return_False) {
        const char* argv[] = {"program", "--input", kInputFileName};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName(kInputFileName)).Times(1).WillOnce(testing::Return(kInputFileName));
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName(_)).Times(0);

        EXPECT_FALSE(cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments));
    }

    TEST_F(CmdArgumentsParserTest, Provide_Only_Output_File_And_Return_False) {
        const char* argv[] = {"program", "--output", kOutputFileName};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName(_)).Times(0);
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName(kOutputFileName)).Times(1).WillOnce(testing::Return(kOutputFileName));

        EXPECT_FALSE(cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments));
    }

    TEST_F(CmdArgumentsParserTest, Print_List_Option_Should_Invoke_ListAvailableInstructions_And_Return_False) {
        const char* argv[] = {"program", "--list"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));
        EXPECT_CALL(*argument_parser_logic_mock, ListAvailableInstructions()).Times(1);

        EXPECT_FALSE(cmd_arguments_parser_with_injected_logic.Parse(cmd_arguments));
    }

    TEST_F(CmdArgumentsParserTest, Provide_Valid_Input_And_Output_Files_And_Values_In_CmdArguments_Are_Set) {
        const char* argv[] = {"program", "--input", kInputFileName, "--output", kOutputFileName};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CmdArgumentsParserWithInjectedLogic cmd_arguments_parser_with_injected_logic2 =
            CmdArgumentsParserWithInjectedLogic(argc, const_cast<char**>(argv), std::unique_ptr<cmd::IArgumentsParserLogic>(argument_parser_logic_mock));

        EXPECT_CALL(*argument_parser_logic_mock, GetInputFileName(kInputFileName)).Times(1).WillOnce(testing::Return(kInputFileName));
        EXPECT_CALL(*argument_parser_logic_mock, GetOutputFileName(kOutputFileName)).Times(1).WillOnce(testing::Return(kOutputFileName));
        cmd_arguments_parser_with_injected_logic2.Parse(cmd_arguments);

        EXPECT_EQ(cmd_arguments->input_file_path.value(), kInputFileName);
        EXPECT_EQ(cmd_arguments->output_file_path.value(), kOutputFileName);
    }

}  // namespace cmd_arguments_parser_test