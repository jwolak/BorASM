#include <gtest/gtest.h>

#include "CharacterStringLineHandlerMock.h"
#include "CodeAnalyzer.h"
#include "FileHandler.h"
#include "InstructionsAssemblerCoreMock.h"

namespace code_analyzer_test {

    using namespace testing;

    class CodeAnalyzerWithInjectedMocks : public assembly_engine::CodeAnalyzer {
      public:
        CodeAnalyzerWithInjectedMocks(std::vector<uint8_t>& machine_code, std::map<std::string, uint16_t>& labels,
                                      std::vector<std::pair<int, std::string>>& label_references,
                                      std::unique_ptr<assembly_engine::ICharacterStringLineHandler> line_handle,
                                      std::unique_ptr<assembly_engine::IInstructionsAssemblerCore> instructions_assembler_core)
            : CodeAnalyzer(machine_code, labels, label_references, std::move(line_handle), std::move(instructions_assembler_core)) {}
    };

    class CodeAnalyzerTest : public ::testing::Test {
      public:
        CodeAnalyzerTest()
            : machine_code_{},
              labels_{},
              label_references_{},
              line_handler_mock_{new mocks::CharacterStringLineHandlerMock},
              instructions_assembler_core_mock_{new mocks::InstructionsAssemblerCoreMock},
              code_analyzer_with_injected_mocks{machine_code_, labels_, label_references_,
                                                std::unique_ptr<assembly_engine::ICharacterStringLineHandler>(line_handler_mock_),
                                                std::unique_ptr<assembly_engine::IInstructionsAssemblerCore>(instructions_assembler_core_mock_)} {}

        std::vector<uint8_t> machine_code_;
        std::map<std::string, uint16_t> labels_;
        std::vector<std::pair<int, std::string>> label_references_;
        mocks::CharacterStringLineHandlerMock* line_handler_mock_;
        mocks::InstructionsAssemblerCoreMock* instructions_assembler_core_mock_;
        CodeAnalyzerWithInjectedMocks code_analyzer_with_injected_mocks;
    };

    TEST_F(CodeAnalyzerTest, Detect_Labels_In_Input_File_And_Detected_Label_Is_In_Labels_And_Label_Address_Is_Sets) {
        std::string temp_file = "input1.asm";
        {
            std::ofstream out(temp_file);
            out << "loop:\nMOV A, B\n";
        }
        std::ifstream input_stream(temp_file);
        std::string line_code;
        EXPECT_CALL(*line_handler_mock_, CleanLineWhitespaces(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) { return input_line; }));
        EXPECT_CALL(*line_handler_mock_, RemoveLineComments(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) { return input_line; }));
        EXPECT_CALL(*line_handler_mock_, TokenizeLine(_)).WillRepeatedly(Return(std::vector<std::string>{"MOV", "A", "B"}));
        EXPECT_CALL(*instructions_assembler_core_mock_, AssembleInstruction(_)).WillOnce(::testing::Return());

        EXPECT_TRUE(code_analyzer_with_injected_mocks.DetectLabels(input_stream, line_code));
        EXPECT_EQ(labels_.count("loop"), 1);
        EXPECT_EQ(labels_["loop"], 0);  // address

        input_stream.close();
        std::remove(temp_file.c_str());
    }

    TEST_F(CodeAnalyzerTest, Try_Detect_Labels_But_Assemble_Instruction_Failed_And_ReturnsFalse) {
        std::string temp_file = "input2.asm";
        {
            std::ofstream out(temp_file);
            out << "MOV A, B\n";
        }
        std::ifstream input_stream(temp_file);
        std::string line_code;
        EXPECT_CALL(*line_handler_mock_, CleanLineWhitespaces(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) { return input_line; }));
        EXPECT_CALL(*line_handler_mock_, RemoveLineComments(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) { return input_line; }));
        EXPECT_CALL(*line_handler_mock_, TokenizeLine(_)).WillRepeatedly(::testing::Return(std::vector<std::string>{"MOV", "A", "B"}));
        EXPECT_CALL(*instructions_assembler_core_mock_, AssembleInstruction(_)).WillOnce(::testing::Throw(std::runtime_error("error")));

        EXPECT_FALSE(code_analyzer_with_injected_mocks.DetectLabels(input_stream, line_code));

        input_stream.close();
        std::remove(temp_file.c_str());
    }

    TEST_F(CodeAnalyzerTest, Detect_Labels_And_Detected_Label_Is_Successfull_And_Returns_True) {
        std::string temp_file = "input3.asm";
        {
            std::ofstream out(temp_file);
            out << " ; komentarz\n\nloop:\n";
        }
        std::ifstream input_stream(temp_file);
        std::string line_code;
        EXPECT_CALL(*line_handler_mock_, CleanLineWhitespaces(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) { return input_line; }));
        EXPECT_CALL(*line_handler_mock_, RemoveLineComments(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) {
            if (input_line.find(';') != std::string::npos) return std::string{};
            return input_line;
        }));
        EXPECT_CALL(*line_handler_mock_, TokenizeLine(_)).WillRepeatedly(::testing::Return(std::vector<std::string>{}));

        EXPECT_TRUE(code_analyzer_with_injected_mocks.DetectLabels(input_stream, line_code));

        input_stream.close();
        std::remove(temp_file.c_str());
    }

    TEST_F(CodeAnalyzerTest, Detect_Labels_And_Detected_Label_Is_Successfull_And_Label_And_Addres_Added_To_Labels) {
        std::string temp_file = "input4.asm";
        {
            std::ofstream out(temp_file);
            out << "loop:\nMOV A, B\n";
        }
        std::ifstream input_file(temp_file);
        std::string line_code;
        EXPECT_CALL(*line_handler_mock_, CleanLineWhitespaces(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) { return input_line; }));
        EXPECT_CALL(*line_handler_mock_, RemoveLineComments(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) { return input_line; }));
        EXPECT_CALL(*line_handler_mock_, TokenizeLine(_)).WillRepeatedly(::testing::Invoke([](const std::string& input_line) {
            if (input_line == "MOV A, B") return std::vector<std::string>{"MOV", "A", "B"};
            return std::vector<std::string>{};
        }));
        EXPECT_CALL(*instructions_assembler_core_mock_, AssembleInstruction(_)).WillOnce(::testing::Return());
        code_analyzer_with_injected_mocks.DetectLabels(input_file, line_code);

        EXPECT_EQ(labels_.count("loop"), 1);
        EXPECT_EQ(labels_["loop"], 0);
        input_file.close();
        std::remove(temp_file.c_str());
    }

}  // namespace code_analyzer_test