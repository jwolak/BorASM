#include <gtest/gtest.h>

#include "CharacterStringLineHandler.h"

namespace character_string_line_handler_test {

    class CharacterStringLineHandlerTest : public ::testing::Test {
      public:
        CharacterStringLineHandlerTest() : character_string_line_handler{} {}

        assembly_engine::CharacterStringLineHandler character_string_line_handler;
    };

    TEST_F(CharacterStringLineHandlerTest, Try_Clean_Line_Whitespaces_Without_Any_Whitespaces_Returns_Same_Line) {
        std::string line_without_whitespaces = "MOV A, B";
        std::string cleaned_line = character_string_line_handler.CleanLineWhitespaces(line_without_whitespaces);

        EXPECT_EQ(cleaned_line, line_without_whitespaces);
    }

    TEST_F(CharacterStringLineHandlerTest, Clean_Line_Whitespaces_Removes_Leading_And_Trailing_Whitespaces) {
        std::string line_with_whitespaces = "   MOV A, B   ";
        std::string cleaned_line = character_string_line_handler.CleanLineWhitespaces(line_with_whitespaces);

        EXPECT_EQ(cleaned_line, "MOV A, B");
    }

    TEST_F(CharacterStringLineHandlerTest, Clean_Line_Whitespaces_Removes_Tabs_LineFeed_Whitespaces) {
        std::string line_with_whitespaces = "\t\tMOV A, B\n\r   ";
        std::string cleaned_line = character_string_line_handler.CleanLineWhitespaces(line_with_whitespaces);

        EXPECT_EQ(cleaned_line, "MOV A, B");
    }

    TEST_F(CharacterStringLineHandlerTest, Remove_Line_Comments_Removes_Comments_From_Line) {
        std::string line_with_comment = "MOV A, B ; This is a comment";
        std::string line_without_comment = character_string_line_handler.RemoveLineComments(line_with_comment);

        EXPECT_EQ(line_without_comment, "MOV A, B ");
    }

    TEST_F(CharacterStringLineHandlerTest, Remove_Line_Comments_Removes_Double_Slash_Comments_From_Line) {
        std::string line_with_comment = "MOV A, B // This is a comment";
        std::string line_without_comment = character_string_line_handler.RemoveLineComments(line_with_comment);

        EXPECT_EQ(line_without_comment, "MOV A, B ");
    }

    TEST_F(CharacterStringLineHandlerTest, IsNumber_Returns_True_For_Valid_Number_String) {
        std::string valid_number = "123";
        EXPECT_TRUE(character_string_line_handler.IsNumber(valid_number));
    }

    TEST_F(CharacterStringLineHandlerTest, IsNumber_Returns_False_For_Empty_String) {
        std::string empty_string = "";
        EXPECT_FALSE(character_string_line_handler.IsNumber(empty_string));
    }

    TEST_F(CharacterStringLineHandlerTest, IsNumber_With_Hash_Prefix_Returns_True_For_Valid_Number_String) {
        std::string valid_number = "#45";
        EXPECT_TRUE(character_string_line_handler.IsNumber(valid_number));
    }

    TEST_F(CharacterStringLineHandlerTest, IsNumber_Returns_False_For_Invalid_Number_String) {
        std::string invalid_number = "12A3";
        EXPECT_FALSE(character_string_line_handler.IsNumber(invalid_number));
    }

    TEST_F(CharacterStringLineHandlerTest, ConvertStringToNumber_Converts_Valid_Number_String_To_Number) {
        std::string valid_number = "255";
        uint8_t number = character_string_line_handler.ConvertStringToNumber(valid_number);

        EXPECT_EQ(number, 255);
    }

    TEST_F(CharacterStringLineHandlerTest, ConvertStringToNumber_Hex_String_To_Number) {
        std::string hex_number = "0x1A";
        uint8_t number = character_string_line_handler.ConvertStringToNumber(hex_number);

        EXPECT_EQ(number, 26);
    }

    TEST_F(CharacterStringLineHandlerTest, ConvertStringToNumber_Remove_Hash_And_Convert_To_Number) {
        std::string immediate_number = "#42";
        uint8_t number = character_string_line_handler.ConvertStringToNumber(immediate_number);

        EXPECT_EQ(number, 42);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Splits_Line_Into_Tokens) {
        std::string line_to_tokenize = "MOV A, B";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"MOV", "A", "B"};
        EXPECT_EQ(tokens, expected_tokens);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Handles_Multiple_Spaces_Between_Tokens) {
        std::string line_to_tokenize = "MOV    A,     B";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"MOV", "A", "B"};
        EXPECT_EQ(tokens, expected_tokens);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Handles_Tabs_Between_Tokens) {
        std::string line_to_tokenize = "MOV\tA,\tB";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"MOV", "A", "B"};
        EXPECT_EQ(tokens, expected_tokens);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Returns_Empty_Vector_For_Empty_Line) {
        std::string empty_line = "";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(empty_line);

        EXPECT_TRUE(tokens.empty());
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Returns_Empty_Vector_For_Line_With_Only_Whitespaces) {
        std::string whitespace_line = "     \t   ";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(whitespace_line);

        EXPECT_TRUE(tokens.empty());
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Handles_Leading_And_Trailing_Whitespaces) {
        std::string line_to_tokenize = "   MOV A, B   ";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"MOV", "A", "B"};
        EXPECT_EQ(tokens, expected_tokens);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Handles_Consecutive_Whitespace_Characters) {
        std::string line_to_tokenize = "MOV\t   A,    B";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"MOV", "A", "B"};
        EXPECT_EQ(tokens, expected_tokens);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Handles_Single_Token_Line) {
        std::string line_to_tokenize = "   HALT   ";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"HALT"};
        EXPECT_EQ(tokens, expected_tokens);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Handles_Line_With_Special_Characters) {
        std::string line_to_tokenize = "MOV A, B; ADD #5";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"MOV", "A", "B;", "ADD", "#5"};
        EXPECT_EQ(tokens, expected_tokens);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Handles_Line_With_No_Spaces_Between_Tokens) {
        std::string line_to_tokenize = "MOVA,B";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"MOVA,B"};
        EXPECT_EQ(tokens, expected_tokens);
    }

    TEST_F(CharacterStringLineHandlerTest, TokenizeLine_Handles_Line_With_Mixed_Whitespace_Characters) {
        std::string line_to_tokenize = "  MOV\tA, \nB  ";
        std::vector<std::string> tokens = character_string_line_handler.TokenizeLine(line_to_tokenize);

        std::vector<std::string> expected_tokens = {"MOV", "A", "B"};
        EXPECT_EQ(tokens, expected_tokens);
    }

}  // namespace character_string_line_handler_test
