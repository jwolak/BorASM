#include "InstructionsAssemblerCore.h"

#include <algorithm>
#include <stdexcept>

#include "CpuOperationCodes.h"
#include "CpuRegisters.h"
#include "Tools.h"
#include "EquinoxLogger.hpp"

namespace assembly_engine {

    InstructionsAssemblerCore::InstructionsAssemblerCore(std::vector<uint8_t>& machine_code, std::vector<std::pair<int, std::string>>& label_references)
        : machine_code_(machine_code), label_references_(label_references), character_string_line_handler_(std::make_unique<CharacterStringLineHandler>()) {}

    /* For tests purposes */
    InstructionsAssemblerCore::InstructionsAssemblerCore(std::vector<uint8_t>& machine_code, std::vector<std::pair<int, std::string>>& label_references,
                                                         std::unique_ptr<ICharacterStringLineHandler> line_handler)
        : machine_code_(machine_code), label_references_(label_references), character_string_line_handler_(std::move(line_handler)) {}

    bool InstructionsAssemblerCore::AssembleInstruction(const std::vector<std::string>& tokens) {
        LOG_TRACE("[InstructionsAssemblerCore] AssembleInstruction() called with " + std::to_string(tokens.size()) + " tokens [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenAssemblingMessage("Assembling instruction...");

        if (tokens.empty()) {
            LOG_DEBUG("[InstructionsAssemblerCore] No tokens provided, skipping assembly. [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintRedAssemblingMessage("No tokens provided, skipping assembly.");
            LOG_DEBUG("[InstructionsAssemblerCore] AssembleInstruction() finished with no action [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            LOG_WARNING("[InstructionsAssemblerCore] Returning true... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return true;
        }

        std::string mnemonic = tokens[0];
        LOG_DEBUG("[InstructionsAssemblerCore] Assembling instruction: " + mnemonic + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), ::toupper);
        LOG_DEBUG("[InstructionsAssemblerCore] Mnemonic after uppercase conversion: " + mnemonic + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_TRACE("[InstructionsAssemblerCore] Checking if mnemonic " + mnemonic + " is valid [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (cpu_data::opcodes.find(mnemonic) == cpu_data::opcodes.end()) {
            LOG_DEBUG("[InstructionsAssemblerCore] Invalid mnemonic: " + mnemonic + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintRedAssemblingMessage("Unknown instruction: " + mnemonic);
            return false;
        }

        uint8_t opcode = cpu_data::opcodes[mnemonic];
        LOG_DEBUG("[InstructionsAssemblerCore] Opcode for mnemonic " + mnemonic + " is " + std::to_string(static_cast<int>(opcode)) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_DEBUG("[InstructionsAssemblerCore] Checking for HALT instruction [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (mnemonic == "HALT") {
            LOG_DEBUG("[InstructionsAssemblerCore] HALT instruction found [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            machine_code_.push_back(opcode);
            tools::PrintGreenAssemblingMessage("[HALT] with opcode: [" + std::to_string(opcode) + "] instruction assembled successfully.");
            LOG_DEBUG("[InstructionsAssemblerCore] HALT instruction assembled and added to machine code [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return true;
        }

        // Jump instructions - always with immediate
        LOG_DEBUG("[InstructionsAssemblerCore] Checking for JMP instruction [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenAssemblingMessage("Checking for JMP instruction...");
        if (opcode >= 0x08 && opcode <= 0x0E) {
            tools::PrintGreenAssemblingMessage("[JMP] [" + std::to_string(opcode) + "] instruction detected.");
            if (tokens.size() < 2) {
                LOG_DEBUG("[InstructionsAssemblerCore] JMP instruction requires address [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                tools::PrintRedAssemblingMessage("JMP instruction requires address");
                return false;
            }

            LOG_DEBUG("[InstructionsAssemblerCore] Assembling JMP instruction [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            machine_code_.push_back(opcode << 4);  // Upper 4 bits = opcode
            tools::PrintGreenAssemblingMessage("[JMP as upper 4 bits] [" + std::to_string(opcode << 4) + "] added to machine code successfully.");
            LOG_DEBUG("[InstructionsAssemblerCore] JMP instruction assembled and added to machine code [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

            LOG_DEBUG("[InstructionsAssemblerCore] Processing jump address/token: " + tokens[1] + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            if (character_string_line_handler_->IsNumber(tokens[1])) {
                LOG_DEBUG("[InstructionsAssemblerCore] Jump address is a number [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                machine_code_.push_back(character_string_line_handler_->ConvertStringToNumber(tokens[1]));
                tools::PrintGreenAssemblingMessage("[JMP as address] [" + tokens[1] + "] added to machine code successfully.");
                LOG_DEBUG("[InstructionsAssemblerCore] Jump address added to machine code [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            } else {
                // Label - remember for later resolution
                LOG_DEBUG("[InstructionsAssemblerCore] Jump address is a label, adding to label references [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                label_references_.push_back({machine_code_.size(), tokens[1]});
                tools::PrintGreenAssemblingMessage("[JMP as label] [" + tokens[1] + "] added to label references successfully.");
                LOG_DEBUG("[InstructionsAssemblerCore] Placeholder added for jump address [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                machine_code_.push_back(0x00);  // Placeholder
                tools::PrintGreenAssemblingMessage("[JMP as placeholder] [0x00] added to machine code successfully.");
            }
            LOG_DEBUG("[InstructionsAssemblerCore] Jump instruction assembled and added to machine code [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Jump instruction assembled successfully.");
            return true;
        }

        // Shift instructions (SHL, SHR) - only one register
        tools::PrintGreenAssemblingMessage("Checking for SHL/SHR instruction...");
        LOG_DEBUG("[InstructionsAssemblerCore] Checking for shift instruction [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (mnemonic == "SHL" || mnemonic == "SHR") {
            tools::PrintGreenAssemblingMessage("Shift instruction detected: " + mnemonic);
            if (tokens.size() < 2) {
                LOG_DEBUG("[InstructionsAssemblerCore] Shift instruction requires register [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                tools::PrintRedAssemblingMessage("Shift instruction requires register");
                return false;
            }

            LOG_DEBUG("[InstructionsAssemblerCore] Assembling shift instruction [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            if (cpu_data::registers.find(tokens[1]) == cpu_data::registers.end()) {
                LOG_DEBUG("[InstructionsAssemblerCore] Invalid register: " + tokens[1] + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                tools::PrintRedAssemblingMessage("Invalid register: " + tokens[1]);
                return false;
            }
            tools::PrintGreenAssemblingMessage("Register " + tokens[1] + " is valid.");
            LOG_DEBUG("[InstructionsAssemblerCore] Shift instruction assembled successfully [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Shift instruction assembled successfully.");

            uint8_t reg = cpu_data::registers[tokens[1]];
            LOG_DEBUG("[InstructionsAssemblerCore] Register " + tokens[1] + " has code " + std::to_string(static_cast<int>(reg)) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Register " + tokens[1] + " has code " + std::to_string(static_cast<int>(reg)) + ".");

            uint8_t instruction = (opcode << 4) | (reg << 2);  // [4-bit opcode][2-bit reg][2-bit unused]
            LOG_DEBUG("[InstructionsAssemblerCore] Shift instruction byte: " + std::to_string(static_cast<int>(instruction)) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Shift instruction byte: " + std::to_string(static_cast<int>(instruction)));

            machine_code_.push_back(instruction);
            LOG_DEBUG("[InstructionsAssemblerCore] Shift instruction added to machine code [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Shift instruction added to machine code successfully.");
            return true;
        }

        // Other instructions - format: INSTR dst, src/imm
        LOG_DEBUG("[InstructionsAssemblerCore] Assembling general instruction [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenAssemblingMessage("Assembling general instruction: [" + mnemonic + "]");
        if (tokens.size() < 3) {
            LOG_DEBUG("[InstructionsAssemblerCore] Instruction requires destination and source [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintRedAssemblingMessage("Instruction requires destination and source");
            return false;
        }

        // Destination register
        LOG_DEBUG("[InstructionsAssemblerCore] Processing destination register: " + tokens[1] + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenAssemblingMessage("Processing destination register: " + tokens[1]);
        if (cpu_data::registers.find(tokens[1]) == cpu_data::registers.end()) {
            LOG_DEBUG("[InstructionsAssemblerCore] Invalid destination register: " + tokens[1] + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintRedErrorMessage("Invalid destination register: " + tokens[1]);
            return false;
        }
        tools::PrintGreenAssemblingMessage("Destination register " + tokens[1] + " is valid.");
        uint8_t destReg = cpu_data::registers[tokens[1]];
        tools::PrintGreenAssemblingMessage("Destination register " + tokens[1] + " has code " + std::to_string(static_cast<int>(destReg)) + ".");
        LOG_DEBUG("[InstructionsAssemblerCore] Destination register " + tokens[1] + " has code " + std::to_string(static_cast<int>(destReg)) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        // Source - register or immediate
        LOG_DEBUG("[InstructionsAssemblerCore] Processing source operand: " + tokens[2] + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenAssemblingMessage("Processing source operand: " + tokens[2]);
        if (character_string_line_handler_->IsNumber(tokens[2]) || tokens[2][0] == '#') {
            // Immediate value
            LOG_DEBUG("[InstructionsAssemblerCore] Source operand is an immediate value [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Source operand is an immediate value: " + tokens[2]);
            uint8_t immediate = character_string_line_handler_->ConvertStringToNumber(tokens[2]);
            LOG_DEBUG("[InstructionsAssemblerCore] Immediate value: " + std::to_string(static_cast<int>(immediate)) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Immediate value: " + std::to_string(static_cast<int>(immediate)));
            uint8_t instruction = (opcode << 4) | (destReg << 2) | immediate;  // [4-bit opcode][2-bit dest][2-bit imm_flag]
            LOG_DEBUG("[InstructionsAssemblerCore] Instruction byte with immediate: " + std::to_string(static_cast<int>(instruction)) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Instruction byte with immediate: " + std::to_string(static_cast<int>(instruction)));

            machine_code_.push_back(instruction);
            LOG_DEBUG("[InstructionsAssemblerCore] Instruction with immediate byte added to machine code [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            machine_code_.push_back(immediate);
            tools::PrintGreenAssemblingMessage("Immediate value " + std::to_string(static_cast<int>(immediate)) + " added to machine code successfully.");
            LOG_DEBUG("[InstructionsAssemblerCore] Instruction with immediate added to machine code [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        } else {
            // Register to register
            LOG_DEBUG("[InstructionsAssemblerCore] Source operand is a register [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Source operand is a register: " + tokens[2]);
            if (cpu_data::registers.find(tokens[2]) == cpu_data::registers.end()) {
                LOG_DEBUG("[InstructionsAssemblerCore] Invalid source register: " + tokens[2] + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                tools::PrintRedAssemblingMessage("Invalid source register: " + tokens[2]);
                return false;
            }
            uint8_t srcReg = cpu_data::registers[tokens[2]];
            tools::PrintGreenAssemblingMessage("Source register " + tokens[2] + " is valid.");
            tools::PrintGreenAssemblingMessage("Source register " + tokens[2] + " has code " + std::to_string(static_cast<int>(srcReg)) + ".");
            LOG_DEBUG("[InstructionsAssemblerCore] Source register " + tokens[2] + " has code " + std::to_string(static_cast<int>(srcReg)) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            uint8_t instruction = (opcode << 4) | (destReg << 2) | srcReg;
            LOG_DEBUG("[InstructionsAssemblerCore] Instruction byte with register: " + std::to_string(static_cast<int>(instruction)) + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Instruction byte with register: " + std::to_string(static_cast<int>(instruction)));
            machine_code_.push_back(instruction);
            LOG_DEBUG("[InstructionsAssemblerCore] Instruction with register added to machine code [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            tools::PrintGreenAssemblingMessage("Instruction with register added to machine code successfully.");
        }

        LOG_DEBUG("[InstructionsAssemblerCore] Finished processing instruction [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenAssemblingMessage("Instruction assembled successfully.");
        return true;
    }
}  // namespace assembly_engine
