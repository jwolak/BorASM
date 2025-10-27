#include <gtest/gtest.h>

#include "CharacterStringLineHandlerMock.h"
#include "CpuOperationCodes.h"
#include "CpuRegisters.h"
#include "InstructionsAssemblerCore.h"

namespace instructions_assembler_core_test {

    class InstructionsAssemblerCoreWithInjectedMocks : public assembly_engine::InstructionsAssemblerCore {
      public:
        InstructionsAssemblerCoreWithInjectedMocks(std::vector<uint8_t>& machine_code, std::vector<std::pair<int, std::string>>& label_references,
                                                   std::unique_ptr<assembly_engine::ICharacterStringLineHandler> line_handler)
            : InstructionsAssemblerCore(machine_code, label_references, std::move(line_handler)) {}
    };

    class InstructionsAssemblerCoreTest : public ::testing::Test {
      public:
        InstructionsAssemblerCoreTest()
            : machine_code{},
              label_references{},
              character_string_line_handler_mock{new mocks::CharacterStringLineHandlerMock},
              instructions_assembler_core_with_injected_mocks{
                  machine_code, label_references, std::unique_ptr<assembly_engine::ICharacterStringLineHandler>(character_string_line_handler_mock)} {}

        std::vector<uint8_t> machine_code;
        std::vector<std::pair<int, std::string>> label_references;
        mocks::CharacterStringLineHandlerMock* character_string_line_handler_mock;
        InstructionsAssemblerCoreWithInjectedMocks instructions_assembler_core_with_injected_mocks;
    };

    TEST_F(InstructionsAssemblerCoreTest, Try_Assemble_Instruction_With_Empty_Tokens_And_Returns_True) {
        std::vector<std::string> tokens;
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
    }

    TEST_F(InstructionsAssemblerCoreTest, Try_Assemble_Instruction_With_Invalid_Mnemonic_And_Returns_False) {
        std::vector<std::string> tokens = {"INVALID_MNEMONIC"};
        EXPECT_FALSE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_ADD_Mnemonic_With_Two_Registers_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"ADD", "R0", "R1"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);  // Assuming ADD R0, R1 opcode is 0x22
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_ADD_Mnemonic_With_One_Register_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"ADD", "R0", "7"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t immediate = static_cast<uint8_t>(std::stoi(tokens[2]));  // Immediate value
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber(tokens[2])).Times(1).WillOnce(::testing::Return(immediate));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | immediate);  // Assuming ADD R0, R1 opcode is 0x22
        EXPECT_EQ(machine_code[1], immediate);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_SUB_Mnemonic_With_Two_Registers_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"SUB", "R0", "R1"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_SUB_Mnemonic_With_One_Register_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"SUB", "R0", "7"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t immediate = static_cast<uint8_t>(std::stoi(tokens[2]));  // Immediate value
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber(tokens[2])).Times(1).WillOnce(::testing::Return(immediate));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | immediate);
        EXPECT_EQ(machine_code[1], immediate);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_AND_Mnemonic_With_Two_Registers_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"AND", "R0", "R1"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_AND_Mnemonic_With_One_Register_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"AND", "R0", "7"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t immediate = static_cast<uint8_t>(std::stoi(tokens[2]));  // Immediate value
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber(tokens[2])).Times(1).WillOnce(::testing::Return(immediate));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | immediate);
        EXPECT_EQ(machine_code[1], immediate);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_OR_Mnemonic_With_Two_Registers_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"OR", "R0", "R1"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_OR_Mnemonic_With_One_Register_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"OR", "R0", "7"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t immediate = static_cast<uint8_t>(std::stoi(tokens[2]));  // Immediate value
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber(tokens[2])).Times(1).WillOnce(::testing::Return(immediate));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | immediate);
        EXPECT_EQ(machine_code[1], immediate);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_XOR_Mnemonic_With_Two_Registers_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"XOR", "R0", "R1"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_XOR_Mnemonic_With_One_Register_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"XOR", "R0", "7"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t immediate = static_cast<uint8_t>(std::stoi(tokens[2]));  // Immediate value
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber(tokens[2])).Times(1).WillOnce(::testing::Return(immediate));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | immediate);
        EXPECT_EQ(machine_code[1], immediate);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_MOV_Mnemonic_With_Two_Registers_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"MOV", "R0", "R1"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_MOV_Mnemonic_With_One_Register_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"MOV", "R0", "7"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t immediate = static_cast<uint8_t>(std::stoi(tokens[2]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber(tokens[2])).Times(1).WillOnce(::testing::Return(immediate));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | immediate);
        EXPECT_EQ(machine_code[1], immediate);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JMP_Mnemonic_With_Number_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JMP", "100"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t jump_address = static_cast<uint8_t>(std::stoi(tokens[1]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("100")).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("100")).WillOnce(::testing::Return(100));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], jump_address);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JMP_Mnemonic_With_Label_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JMP", "LABEL"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("LABEL")).WillOnce(::testing::Return(false));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("LABEL")).Times(0);
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], 0);            // LABEL address should be resolved to 0
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JZ_Mnemonic_With_Number_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JZ", "100"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t jump_address = static_cast<uint8_t>(std::stoi(tokens[1]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("100")).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("100")).WillOnce(::testing::Return(100));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], jump_address);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JZ_Mnemonic_With_Label_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JZ", "LABEL"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("LABEL")).WillOnce(::testing::Return(false));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("LABEL")).Times(0);
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], 0);            // LABEL address should be resolved to 0
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JNZ_Mnemonic_With_Number_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JNZ", "100"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t jump_address = static_cast<uint8_t>(std::stoi(tokens[1]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("100")).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("100")).WillOnce(::testing::Return(100));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], jump_address);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JNZ_Mnemonic_With_Label_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JNZ", "LABEL"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("LABEL")).WillOnce(::testing::Return(false));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("LABEL")).Times(0);
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], 0);            // LABEL address should be resolved to 0
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JC_Mnemonic_With_Number_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JC", "100"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t jump_address = static_cast<uint8_t>(std::stoi(tokens[1]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("100")).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("100")).WillOnce(::testing::Return(100));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], jump_address);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JC_Mnemonic_With_Label_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JC", "LABEL"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("LABEL")).WillOnce(::testing::Return(false));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("LABEL")).Times(0);
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], 0);            // LABEL address should be resolved to 0
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JNC_Mnemonic_With_Number_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JNC", "100"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t jump_address = static_cast<uint8_t>(std::stoi(tokens[1]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("100")).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("100")).WillOnce(::testing::Return(100));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], jump_address);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JNC_Mnemonic_With_Label_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JNC", "LABEL"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("LABEL")).WillOnce(::testing::Return(false));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("LABEL")).Times(0);
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], 0);            // LABEL address should be resolved to 0
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JN_Mnemonic_With_Number_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JN", "100"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t jump_address = static_cast<uint8_t>(std::stoi(tokens[1]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("100")).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("100")).WillOnce(::testing::Return(100));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], jump_address);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JN_Mnemonic_With_Label_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JN", "LABEL"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("LABEL")).WillOnce(::testing::Return(false));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("LABEL")).Times(0);
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], 0);            // LABEL address should be resolved to 0
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JNN_Mnemonic_With_Number_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JNN", "100"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t jump_address = static_cast<uint8_t>(std::stoi(tokens[1]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("100")).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("100")).WillOnce(::testing::Return(100));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], jump_address);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_JNN_Mnemonic_With_Label_As_Address_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"JNN", "LABEL"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber("LABEL")).WillOnce(::testing::Return(false));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber("LABEL")).Times(0);
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], opcode << 4);  // machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
        EXPECT_EQ(machine_code[1], 0);            // LABEL address should be resolved to 0
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_SHL_Mnemonic_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"SHL", "R0"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg = cpu_data::registers[tokens[1]];
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg << 2));  // Assuming SHL R0 opcode is 0x20
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_SHR_Mnemonic_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"SHR", "R0"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg = cpu_data::registers[tokens[1]];
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg << 2));  // Assuming SHR R0 opcode is 0x21
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_CMP_Two_Registers_Mnemonic_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"CMP", "R0", "R1"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(testing::_)).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);  // Assuming CMP R0, R1 opcode is 0x30
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_CMP_With_Immediate_Mnemonic_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"CMP", "R0", "7"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t immediate = static_cast<uint8_t>(std::stoi(tokens[2]));
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).WillOnce(::testing::Return(true));
        EXPECT_CALL(*character_string_line_handler_mock, ConvertStringToNumber(tokens[2])).WillOnce(::testing::Return(7));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 2);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | immediate);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_HALT_Mnemonic_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"HALT"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], opcode);  // HALT opcode is 0xFF
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_AND_Mnemonic_With_Two_Registers_A_B_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"AND", "A", "B"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);
    }

    TEST_F(InstructionsAssemblerCoreTest,
           Assemble_Instruction_With_AND_Mnemonic_With_Two_Registers_R2_R3_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"AND", "R2", "R3"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);
    }

    TEST_F(InstructionsAssemblerCoreTest, Assemble_Instruction_With_AND_Mnemonic_With_Two_Registers_C_D_Successful_And_Machine_Code_Updated_And_True_Returned) {
        std::vector<std::string> tokens = {"AND", "C", "D"};
        uint8_t opcode = cpu_data::opcodes[tokens[0]];
        uint8_t reg1 = cpu_data::registers[tokens[1]];
        uint8_t reg2 = cpu_data::registers[tokens[2]];
        EXPECT_CALL(*character_string_line_handler_mock, IsNumber(tokens[2])).Times(1).WillOnce(::testing::Return(false));
        EXPECT_TRUE(instructions_assembler_core_with_injected_mocks.AssembleInstruction(tokens));
        ASSERT_EQ(machine_code.size(), 1);
        EXPECT_EQ(machine_code[0], (opcode << 4) | (reg1 << 2) | reg2);
    }

}  // namespace instructions_assembler_core_test
