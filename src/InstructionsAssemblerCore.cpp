#include "InstructionsAssemblerCore.h"

#include <algorithm>
#include <stdexcept>

#include "CpuOperationCodes.h"
#include "CpuRegisters.h"

namespace assembly_engine {

    InstructionsAssemblerCore::InstructionsAssemblerCore(std::vector<uint8_t>& machineCode, std::vector<std::pair<int, std::string>>& labelReferences)
        : machineCode(machineCode), labelReferences(labelReferences), character_string_line_handler_(std::make_unique<CharacterStringLineHandler>()) {}

    void InstructionsAssemblerCore::AssembleInstruction(const std::vector<std::string>& tokens) {
        if (tokens.empty()) return;

        std::string mnemonic = tokens[0];
        std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), ::toupper);

        if (cpu_data::opcodes.find(mnemonic) == cpu_data::opcodes.end()) {
            throw std::runtime_error("Unknown instruction: " + mnemonic);
        }

        uint8_t opcode = cpu_data::opcodes[mnemonic];

        // HALT - instrukcja bez parametrów
        if (mnemonic == "HALT") {
            machineCode.push_back(0xFF);
            return;
        }

        // Instrukcje skoków - zawsze z immediate
        if (opcode >= 0x08 && opcode <= 0x0E) {
            if (tokens.size() < 2) {
                throw std::runtime_error("Jump instruction requires address");
            }

            machineCode.push_back(opcode << 4);  // Górne 4 bity = opcode

            if (character_string_line_handler_->IsNumber(tokens[1])) {
                machineCode.push_back(character_string_line_handler_->ConvertStringToNumber(tokens[1]));
            } else {
                // Label - zapamiętaj do późniejszego rozwiązania
                labelReferences.push_back({machineCode.size(), tokens[1]});
                machineCode.push_back(0x00);  // Placeholder
            }
            return;
        }

        // Instrukcje shift (SHL, SHR) - tylko jeden rejestr
        if (mnemonic == "SHL" || mnemonic == "SHR") {
            if (tokens.size() < 2) {
                throw std::runtime_error("Shift instruction requires register");
            }

            if (cpu_data::registers.find(tokens[1]) == cpu_data::registers.end()) {
                throw std::runtime_error("Invalid register: " + tokens[1]);
            }

            uint8_t reg = cpu_data::registers[tokens[1]];
            uint8_t instruction = (opcode << 4) | (reg << 2);  // [4-bit opcode][2-bit reg][2-bit unused]
            machineCode.push_back(instruction);
            return;
        }

        // Pozostałe instrukcje - format: INSTR dst, src/imm
        if (tokens.size() < 3) {
            throw std::runtime_error("Instruction requires destination and source");
        }

        // Rejestr docelowy
        if (cpu_data::registers.find(tokens[1]) == cpu_data::registers.end()) {
            throw std::runtime_error("Invalid destination register: " + tokens[1]);
        }
        uint8_t destReg = cpu_data::registers[tokens[1]];

        // Źródło - rejestr lub immediate
        if (character_string_line_handler_->IsNumber(tokens[2]) || tokens[2][0] == '#') {
            // Immediate value
            uint8_t immediate = character_string_line_handler_->ConvertStringToNumber(tokens[2]);
            uint8_t instruction = (opcode << 4) | (destReg << 2) | immediate;  // [4-bit opcode][2-bit dest][2-bit imm_flag]

            machineCode.push_back(instruction);
            machineCode.push_back(immediate);
        } else {
            // Register to register
            if (cpu_data::registers.find(tokens[2]) == cpu_data::registers.end()) {
                throw std::runtime_error("Invalid source register: " + tokens[2]);
            }
            uint8_t srcReg = cpu_data::registers[tokens[2]];
            uint8_t instruction = (opcode << 4) | (destReg << 2) | srcReg;
            machineCode.push_back(instruction);
        }
    }
}  // namespace assembly_engine
