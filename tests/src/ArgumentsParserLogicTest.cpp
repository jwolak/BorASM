#include <gtest/gtest.h>

#include "ArgumentsParserLogic.h"

namespace arguments_parser_logic_test {

    namespace {
        const std::string kExpectedHelpPrint =
            "BorASM - Assembler CLI\n"
            "Version: 0.1.0\n"
            "\n"
            "Usage:\n"
            "  BorASM [options]\n"
            "\n"
            "Options:\n"
            "  -h, --help                 Show this help message\n"
            "  -v, --version              Show version information\n"
            "  -D, --debug                Enable debug mode\n"
            "  -i, --input <file>         Input assembly source file\n"
            "  -o, --output <file>        Output file name\n"
            "  -l, --list                 List all available instructions\n"
            "\n"
            "Example:\n"
            "  BorASM -i code.asm -o code.bin\n";
    }  // namespace

    class ArgumentsParserLogicTest : public ::testing::Test {
      public:
        void SetUp() override {}

        void TearDown() override {}

        cmd::ArgumentsParserLogic argument_parser_logic_;
    };

    TEST_F(ArgumentsParserLogicTest, PrintHelpTest) {
        testing::internal::CaptureStdout();
        argument_parser_logic_.PrintHelp();
        std::string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, kExpectedHelpPrint);
    }
}  // namespace arguments_parser_logic_test