#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace cpu_data {
    std::map<std::string, uint8_t> opcodes = {
        // Arithmetic instructions (0x00-0x07)
        {"ADD", 0x00},  // ADD reg, reg/imm
        {"SUB", 0x01},  // SUB reg, reg/imm
        {"AND", 0x02},  // AND reg, reg/imm
        {"OR", 0x03},   // OR reg, reg/imm
        {"XOR", 0x04},  // XOR reg, reg/imm
        {"MOV", 0x05},  // MOV reg, reg/imm
        {"SHL", 0x06},  // Shift Left
        {"SHR", 0x07},  // Shift Right

        // Jump instructions (0x08-0x0E)
        {"JMP", 0x08},  // Jump unconditional
        {"JZ", 0x09},   // Jump if Zero
        {"JNZ", 0x0A},  // Jump if Not Zero
        {"JC", 0x0B},   // Jump if Carry
        {"JNC", 0x0C},  // Jump if Not Carry
        {"JN", 0x0D},   // Jump if Negative
        {"JNN", 0x0E},  // Jump if Not Negative

        // Special instructions
        {"CMP", 0x0F},  // Compare
        {"HALT", 0xFF}  // Halt
    };
}