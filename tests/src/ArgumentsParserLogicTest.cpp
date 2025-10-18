#include <gtest/gtest.h>

#include "ArgumentsParserLogic.h"
#include "version.h"

namespace arguments_parser_logic_test {

    namespace {}  // namespace

    class ArgumentsParserLogicTest : public ::testing::Test {
      public:
        void SetUp() override {}

        void TearDown() override {}

        std::string GetExpectedHelpPrint() {
            return std::string("BorASM - Assembler CLI\n") + "Version: " + BorASM::Version::GetVersionString() +
                   "\n"
                   "\n"
                   "Usage:\n"
                   "  BorASM [options]\n"
                   "\nOptions:\n"
                   "  -h, --help                 Show this help message\n"
                   "  -v, --version              Show version information\n"
                   "  -D, --debug                Enable debug mode\n"
                   "  -i, --input <file>         Input assembly source file\n"
                   "  -o, --output <file>        Output file name\n"
                   "  -l, --list                 List all available instructions\n"
                   "\nExample:\n"
                   "  BorASM -i code.asm -o code.bin\n";
        }

        cmd::ArgumentsParserLogic argument_parser_logic_;
    };

    TEST_F(ArgumentsParserLogicTest, PrintHelp_And_Returned_Content_Is_As_Expected) {
        testing::internal::CaptureStdout();
        argument_parser_logic_.PrintHelp();
        std::string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, GetExpectedHelpPrint());
    }
}  // namespace arguments_parser_logic_test