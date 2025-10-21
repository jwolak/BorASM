#include "InstructionsAssemblerCore.h"

#include <algorithm>
#include <stdexcept>

#include "CpuOperationCodes.h"
#include "CpuRegisters.h"
#include "Tools.h"
#include "spdlog/spdlog.h"

namespace assembly_engine {

    InstructionsAssemblerCore::InstructionsAssemblerCore(std::vector<uint8_t>& machine_code_, std::vector<std::pair<int, std::string>>& labelReferences)
        : machine_code_(machine_code_), labelReferences(labelReferences), character_string_line_handler_(std::make_unique<CharacterStringLineHandler>()) {}

    void InstructionsAssemblerCore::AssembleInstruction(const std::vector<std::string>& tokens) {
        spdlog::trace("[InstructionsAssemblerCore] AssembleInstruction() called with {0} tokens [{1}:{2}]", tokens.size(), __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Assembling instruction...");

        if (tokens.empty()) {
            spdlog::debug("[InstructionsAssemblerCore] No tokens provided, skipping assembly [{0}:{1}]", __FILENAME__, __LINE__);
            tools::PrintRedErrorMessage("No tokens provided, skipping assembly.");
            return;
        }

        std::string mnemonic = tokens[0];
        spdlog::debug("[InstructionsAssemblerCore] Assembling instruction: {0} [{1}:{2}]", mnemonic, __FILENAME__, __LINE__);
        std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), ::toupper);
        spdlog::debug("[InstructionsAssemblerCore] Mnemonic after uppercase conversion: {0} [{1}:{2}]", mnemonic, __FILENAME__, __LINE__);

        spdlog::trace("[InstructionsAssemblerCore] Checking if mnemonic {0} is valid [{1}:{2}]", mnemonic, __FILENAME__, __LINE__);
        if (cpu_data::opcodes.find(mnemonic) == cpu_data::opcodes.end()) {
            spdlog::debug("[InstructionsAssemblerCore] Invalid mnemonic: {0} [{1}:{2}]", mnemonic, __FILENAME__, __LINE__);
            tools::PrintRedErrorMessage("Unknown instruction: " + mnemonic);
            throw std::runtime_error("Unknown instruction: " + mnemonic);
        }

        uint8_t opcode = cpu_data::opcodes[mnemonic];
        spdlog::debug("[InstructionsAssemblerCore] Opcode for mnemonic {0} is {1} [{2}:{3}]", mnemonic, static_cast<int>(opcode), __FILENAME__, __LINE__);

        spdlog::debug("[InstructionsAssemblerCore] Checking for HALT instruction [{0}:{1}]", __FILENAME__, __LINE__);
        if (mnemonic == "HALT") {
            spdlog::debug("[InstructionsAssemblerCore] HALT instruction found [{0}:{1}]", __FILENAME__, __LINE__);
            machine_code_.push_back(0xFF);
            spdlog::debug("[InstructionsAssemblerCore] HALT instruction assembled and added to machine code [{0}:{1}]", __FILENAME__, __LINE__);
            return;
        }

        // Jump instructions - always with immediate
        spdlog::debug("[InstructionsAssemblerCore] Checking for jump instruction [{0}:{1}]", __FILENAME__, __LINE__);
        if (opcode >= 0x08 && opcode <= 0x0E) {
            if (tokens.size() < 2) {
                spdlog::debug("[InstructionsAssemblerCore] Jump instruction requires address [{0}:{1}]", __FILENAME__, __LINE__);
                tools::PrintRedErrorMessage("Jump instruction requires address");
                throw std::runtime_error("Jump instruction requires address");
            }

            spdlog::debug("[InstructionsAssemblerCore] Assembling jump instruction [{0}:{1}]", __FILENAME__, __LINE__);
            machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
            spdlog::debug("[InstructionsAssemblerCore] Jump instruction assembled and added to machine code [{0}:{1}]", __FILENAME__, __LINE__);

            spdlog::debug("[InstructionsAssemblerCore] Processing jump address/token: {0} [{1}:{2}]", tokens[1], __FILENAME__, __LINE__);
            if (character_string_line_handler_->IsNumber(tokens[1])) {
                spdlog::debug("[InstructionsAssemblerCore] Jump address is a number [{0}:{1}]", __FILENAME__, __LINE__);
                machine_code_.push_back(character_string_line_handler_->ConvertStringToNumber(tokens[1]));
                spdlog::debug("[InstructionsAssemblerCore] Jump address added to machine code [{0}:{1}]", __FILENAME__, __LINE__);
            } else {
                // Label - remember for later resolution
                spdlog::debug("[InstructionsAssemblerCore] Jump address is a label, adding to label references [{0}:{1}]", __FILENAME__, __LINE__);
                labelReferences.push_back({machine_code_.size(), tokens[1]});
                spdlog::debug("[InstructionsAssemblerCore] Placeholder added for jump address [{0}:{1}]", __FILENAME__, __LINE__);
                machine_code_.push_back(0x00);  // Placeholder
            }
            spdlog::debug("[InstructionsAssemblerCore] Jump instruction assembled and added to machine code [{0}:{1}]", __FILENAME__, __LINE__);
            tools::PrintGreenOKMessage("Jump instruction assembled successfully.");
            return;
        }

        // Shift instructions (SHL, SHR) - only one register
        spdlog::debug("[InstructionsAssemblerCore] Checking for shift instruction [{0}:{1}]", __FILENAME__, __LINE__);
        if (mnemonic == "SHL" || mnemonic == "SHR") {
            if (tokens.size() < 2) {
                spdlog::debug("[InstructionsAssemblerCore] Shift instruction requires register [{0}:{1}]", __FILENAME__, __LINE__);
                throw std::runtime_error("Shift instruction requires register");
            }

            spdlog::debug("[InstructionsAssemblerCore] Assembling shift instruction [{0}:{1}]", __FILENAME__, __LINE__);
            if (cpu_data::registers.find(tokens[1]) == cpu_data::registers.end()) {
                spdlog::debug("[InstructionsAssemblerCore] Invalid register: {0} [{1}:{2}]", tokens[1], __FILENAME__, __LINE__);
                throw std::runtime_error("Invalid register: " + tokens[1]);
            }
            spdlog::debug("[InstructionsAssemblerCore] Shift instruction assembled successfully [{0}:{1}]", __FILENAME__, __LINE__);
            tools::PrintGreenOKMessage("Shift instruction assembled successfully.");

            uint8_t reg = cpu_data::registers[tokens[1]];
            spdlog::debug("[InstructionsAssemblerCore] Register {0} has code {1} [{2}:{3}]", tokens[1], static_cast<int>(reg), __FILENAME__, __LINE__);

            uint8_t instruction = (opcode << 4) | (reg << 2);  // [4-bit opcode][2-bit reg][2-bit unused]
            spdlog::debug("[InstructionsAssemblerCore] Shift instruction byte: {0} [{1}:{2}]", static_cast<int>(instruction), __FILENAME__, __LINE__);

            machine_code_.push_back(instruction);
            spdlog::debug("[InstructionsAssemblerCore] Shift instruction added to machine code [{0}:{1}]", __FILENAME__, __LINE__);

            tools::PrintGreenOKMessage("Shift instruction added to machine code successfully.");
            return;
        }

        // Other instructions - format: INSTR dst, src/imm
        spdlog::debug("[InstructionsAssemblerCore] Assembling general instruction [{0}:{1}]", __FILENAME__, __LINE__);
        if (tokens.size() < 3) {
            spdlog::debug("[InstructionsAssemblerCore] Instruction requires destination and source [{0}:{1}]", __FILENAME__, __LINE__);
            tools::PrintRedErrorMessage("Instruction requires destination and source");
            throw std::runtime_error("Instruction requires destination and source");
        }

        // Destination register
        spdlog::debug("[InstructionsAssemblerCore] Processing destination register: {0} [{1}:{2}]", tokens[1], __FILENAME__, __LINE__);
        if (cpu_data::registers.find(tokens[1]) == cpu_data::registers.end()) {
            spdlog::debug("[InstructionsAssemblerCore] Invalid destination register: {0} [{1}:{2}]", tokens[1], __FILENAME__, __LINE__);
            tools::PrintRedErrorMessage("Invalid destination register: " + tokens[1]);
            throw std::runtime_error("Invalid destination register: " + tokens[1]);
        }
        uint8_t destReg = cpu_data::registers[tokens[1]];
        spdlog::debug("[InstructionsAssemblerCore] Destination register {0} has code {1} [{2}:{3}]", tokens[1], static_cast<int>(destReg), __FILENAME__,
                      __LINE__);

        // Source - register or immediate
        spdlog::debug("[InstructionsAssemblerCore] Processing source operand: {0} [{1}:{2}]", tokens[2], __FILENAME__, __LINE__);
        if (character_string_line_handler_->IsNumber(tokens[2]) || tokens[2][0] == '#') {
            // Immediate value
            spdlog::debug("[InstructionsAssemblerCore] Source operand is an immediate value [{0}:{1}]", __FILENAME__, __LINE__);
            uint8_t immediate = character_string_line_handler_->ConvertStringToNumber(tokens[2]);
            spdlog::debug("[InstructionsAssemblerCore] Immediate value: {0} [{1}:{2}]", static_cast<int>(immediate), __FILENAME__, __LINE__);
            uint8_t instruction = (opcode << 4) | (destReg << 2) | immediate;  // [4-bit opcode][2-bit dest][2-bit imm_flag]
            spdlog::debug("[InstructionsAssemblerCore] Instruction byte with immediate: {0} [{1}:{2}]", static_cast<int>(instruction), __FILENAME__, __LINE__);

            machine_code_.push_back(instruction);
            machine_code_.push_back(immediate);
            spdlog::debug("[InstructionsAssemblerCore] Instruction with immediate added to machine code [{0}:{1}]", __FILENAME__, __LINE__);
        } else {
            // Register to register
            spdlog::debug("[InstructionsAssemblerCore] Source operand is a register [{0}:{1}]", __FILENAME__, __LINE__);
            if (cpu_data::registers.find(tokens[2]) == cpu_data::registers.end()) {
                spdlog::debug("[InstructionsAssemblerCore] Invalid source register: {0} [{1}:{2}]", tokens[2], __FILENAME__, __LINE__);
                tools::PrintRedErrorMessage("Invalid source register: " + tokens[2]);
                throw std::runtime_error("Invalid source register: " + tokens[2]);
            }
            uint8_t srcReg = cpu_data::registers[tokens[2]];
            spdlog::debug("[InstructionsAssemblerCore] Source register {0} has code {1} [{2}:{3}]", tokens[2], static_cast<int>(srcReg), __FILENAME__,
                          __LINE__);
            uint8_t instruction = (opcode << 4) | (destReg << 2) | srcReg;
            spdlog::debug("[InstructionsAssemblerCore] Instruction byte with register: {0} [{1}:{2}]", static_cast<int>(instruction), __FILENAME__, __LINE__);
            machine_code_.push_back(instruction);
            spdlog::debug("[InstructionsAssemblerCore] Instruction with register added to machine code [{0}:{1}]", __FILENAME__, __LINE__);
        }

        spdlog::debug("[InstructionsAssemblerCore] Finished processing instruction [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Instruction assembled successfully.");
    }
}  // namespace assembly_engine
