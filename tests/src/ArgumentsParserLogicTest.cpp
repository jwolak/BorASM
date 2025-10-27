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

        std::string GetExpectedInstructionsPrint() {
            return std::string(
                "Available instructions:\n"
                "\nArithmetic instructions (0x00-0x07):\n"
                "  ADD   0x00   Add:        ADD reg, reg/imm\n"
                "  SUB   0x01   Subtract:   SUB reg, reg/imm\n"
                "  AND   0x02   And:        AND reg, reg/imm\n"
                "  OR    0x03   Or:         OR reg, reg/imm\n"
                "  XOR   0x04   Xor:        XOR reg, reg/imm\n"
                "  MOV   0x05   Move:       MOV reg, reg/imm\n"
                "  SHL   0x06   Shift left: SHL reg, reg/imm\n"
                "  SHR   0x07   Shift right:SHR reg, reg/imm\n"
                "\nJump instructions (0x08-0x0E):\n"
                "  JMP   0x08   Unconditional jump\n"
                "  JZ    0x09   Jump if zero\n"
                "  JNZ   0x0A   Jump if not zero\n"
                "  JC    0x0B   Jump if carry\n"
                "  JNC   0x0C   Jump if not carry\n"
                "  JN    0x0D   Jump if negative\n"
                "  JNN   0x0E   Jump if not negative\n"
                "\nSpecial instructions:\n"
                "  CMP   0x0F   Compare\n"
                "  HALT  0xFF   Halt execution\n");
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

    TEST_F(ArgumentsParserLogicTest, Enable_DebugMode_And_Debug_Mode_Enaled) {
        testing::internal::CaptureStdout();
        bool result = argument_parser_logic_.EnableDebugMode();
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(result);
        EXPECT_NE(output.find("Debug mode enabled"), std::string::npos);
    }

    TEST_F(ArgumentsParserLogicTest, Provide_Input_File_Name_With_Valid_Argument_And_File_Name_Returned) {
        const char* test_input = "input.asm";
        auto result = argument_parser_logic_.GetInputFileName(test_input);
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), test_input);
    }

    TEST_F(ArgumentsParserLogicTest, Provide_Output_File_Name_With_Valid_Argument_And_File_Name_Returned) {
        const char* test_output = "output.bin";
        auto result = argument_parser_logic_.GetOutputFileName(test_output);
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), test_output);
    }

    TEST_F(ArgumentsParserLogicTest, Try_Provide_Invalid_Input_File_Name_And_Returns_Empty) {
        const char* test_input = nullptr;
        auto result = argument_parser_logic_.GetInputFileName(test_input);
        EXPECT_FALSE(result.has_value());
    }

    TEST_F(ArgumentsParserLogicTest, Try_Provide_Invalid_Output_File_Name_And_Returns_Empty) {
        const char* test_output = nullptr;
        auto result = argument_parser_logic_.GetOutputFileName(test_output);
        EXPECT_FALSE(result.has_value());
    }

    TEST_F(ArgumentsParserLogicTest, List_Available_Instructions_And_Content_Is_As_Expected) {
        testing::internal::CaptureStdout();
        argument_parser_logic_.ListAvailableInstructions();
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(output, GetExpectedInstructionsPrint());
    }
}  // namespace arguments_parser_logic_test