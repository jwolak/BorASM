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

        std::string GetExpectedVersionPrint() {
            return std::string("Version: ") + BorASM::Version::GetVersionString() + "\n" + "Full Version: " + BorASM::Version::GetFullVersionString() + "\n" +
                   "Complete Version: " + BorASM::Version::GetCompleteVersionInfo() + "\n" + "Build Type: " + BorASM::Version::BUILD_TYPE + "\n" +
                   "Is version at least 0.1.0 ? " + (BorASM::Version::IsVersionAtLeast(0, 1, 0) ? "[Yes]" : "[No]") + "\n";
        }

        cmd::ArgumentsParserLogic argument_parser_logic_;
    };

    TEST_F(ArgumentsParserLogicTest, PrintHelp_And_Returned_Content_Is_As_Expected) {
        testing::internal::CaptureStdout();
        argument_parser_logic_.PrintHelp();
        std::string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, GetExpectedHelpPrint());
    }

    TEST_F(ArgumentsParserLogicTest, PrintVersion_And_Returned_Content_Is_As_Expected) {
        testing::internal::CaptureStdout();
        argument_parser_logic_.PrintVersionInfo();
        std::string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, GetExpectedVersionPrint());
    }

    TEST_F(ArgumentsParserLogicTest, EnableDebugModeTest) {
        testing::internal::CaptureStdout();
        bool result = argument_parser_logic_.EnableDebugMode();
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(result);
        EXPECT_NE(output.find("Debug mode enabled"), std::string::npos);
    }
}  // namespace arguments_parser_logic_test