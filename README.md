# BorASM

[![Unit Tests](https://github.com/jwolak/BorASM/actions/workflows/tests.yml/badge.svg)](https://github.com/jwolak/BorASM/actions/workflows/tests.yml)

Assembly compiler for Boruss CPU "Laibach"

```
 ____   ___  ____      _    ____  __  __ 
| __ ) / _ \|  _ \    / \  / ___||  \/  |
|  _ \| | | | |_) |  / _ \ \___ \| |\/| |
| |_) | |_| |  _ <  / ___ \ ___) | |  | |
|____/ \___/|_| \_\/_/   \_\____/|_|  |_|

Boruss CPU Assembler v1.0 
Target: "Laibach" Core (8-bit RISC) (see: https://github.com/jwolak/BorussCPU-Laibach)
```

Experimental assembler written in C++ for an experimental RISC BorussCPU "Laibach" - see: https://github.com/jwolak/BorussCPU-Laibach. BorASM reads assembly source files, detects labels and references, and emits a compact hexadecimal machine-code representation. The project includes a small instruction set, unit tests (Google Test / Google Mock), and utilities for parsing and assembling source files.

## Features

- Two-pass assembly (label detection + code emission)
- Support for immediate and register operands
- Label references and resolution
- Unit tests covering parsing, assembly core and file handling

## Repository layout

- `src/` — implementation (assembler core, line handler, file handler, CPU tables)
- `include/` — public headers
- `tests/` — unit tests (Google Test / Google Mock)
- `scripts/` — helper scripts (build/test)
- `CMakeLists.txt` — top-level CMake configuration

## Build (Linux)

Prerequisites: CMake, a C++17 compatible compiler, GoogleTest (recommended to build via CMake targets).

Example (out-of-source):

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j$(nproc)
```

## Run

After building, use the produced binary to assemble `.asm` files:

```bash
./bin/BorASM <input.asm> <output.bin>
```

## Tests

Run unit tests from the `build` directory (CMake adds a `tests` target):

```bash
cmake --build . --target BorASM-Tests.x64
ctest -j$(nproc)
```

Or run the test binary directly in `tests/bin/` after building.

## Example

Source .asm
```
MOV R0, #1  ;LOAD immediate value 1 to R0
loop:
SHL R0      ;(LED0->LED1)
SHL R0      ;(LED1->LED2)
SHL R0      ;(LED2->LED3)
SHL R0      ;(LED3->LED4)
SHR R0      ;(LED4->LED3)
SHR R0      ;(LED3->LED2)
SHR R0      ;(LED2->LED1)
SHR R0      ;(LED1->LED0)
JMP loop
```
Output .hex

```
        |OPER|DATA|     |OPER|DATA|
51       0101 0001       MOV   #1       ;MOV R0, #1 (MOV opcode = 0x05, R0 = 0, data type: immediate(#1); (0x05 << 4) | (0 << 2) | 1 = 0x51). Note: Data type can be immediate(#) or register.
01       0000 0001              1       ;Immediate value: 0x01
60       0110 0000       SHL    R0      ;SHL R0 (SHL opcode = 0x06, R0 = 0; (0x06 << 4) | (0 << 2) = 0x60)
60       0110 0000       SHL    R0
60       0110 0000       SHL    R0
60       0110 0000       SHL    R0
70       0111 0000       SHR    R0      ;SHR R0 (SHL opcode = 0x07, R0 = 0; (0x07 << 4) | (0 << 2) = 0x70)
70       0111 0000       SHR    R0
70       0111 0000       SHR    R0
70       0111 0000       SHR    R0
80       1000 0000       JMP    -      ;JMP loop (JMP opcode = 0x08, (0x08 << 4) = 0x80, label addess = 0x02)
02       0000 0010        -     2      ;Label address (offset: 0x02 - the second instruction -> SHL R0)
```
MOV is alias for:
```
XOR R0, R0              ;R0 cleared. Opcode '0x05' is 'XOR' for BorussCPU (not MOV)
ADD R0, #immediate      ;Add immediate value to R0. Opcode '0x00' is 'ADD' for BorussCPU
```
[See source code for the ALU module: https://github.com/jwolak/BorussCPU-Laibach/blob/main/src/core/boruss_alu.v](https://github.com/jwolak/BorussCPU-Laibach/blob/main/src/core/boruss_alu.v)

## Instruction Set Reference

### Registers

The Laibach CPU has 4 general-purpose 8-bit registers:

| Register | Code | Description |
|----------|------|-------------|
| R0       | 0x00 | General purpose register 0 |
| R1       | 0x01 | General purpose register 1 |
| R2       | 0x02 | General purpose register 2 |
| R3       | 0x03 | General purpose register 3 |

### Operand Types

- **Immediate**: A constant value prefixed with `#` (e.g., `#42`, `#0xFF`)
- **Register**: A register name (e.g., `R0`, `R1`)

### Instruction Encoding

Each instruction is encoded as:
- **Bits 7-4**: Opcode (4 bits)
- **Bits 3-2**: Destination/Source Register (2 bits)
- **Bits 1-0**: Operand Type (2 bits: 01 = immediate, 00 = register)

If immediate value is used, the next byte contains the actual value.

### Complete Opcode Table

#### Arithmetic Instructions (0x00-0x07)

| Mnemonic | Opcode | Syntax | Description |
|----------|--------|--------|-------------|
| ADD      | 0x00   | ADD Rd, Rs/imm | Add register or immediate to destination |
| SUB      | 0x01   | SUB Rd, Rs/imm | Subtract register or immediate from destination |
| AND      | 0x02   | AND Rd, Rs/imm | Bitwise AND |
| OR       | 0x03   | OR Rd, Rs/imm  | Bitwise OR |
| XOR      | 0x04   | XOR Rd, Rs/imm | Bitwise XOR (can be used to clear register) |
| MOV      | 0x05   | MOV Rd, Rs/imm | Move/Copy value to destination |
| SHL      | 0x06   | SHL Rd         | Shift Left (multiply by 2) |
| SHR      | 0x07   | SHR Rd         | Shift Right (divide by 2) |

**Examples:**
```asm
ADD R0, R1         ; R0 = R0 + R1
ADD R0, #10        ; R0 = R0 + 10
XOR R2, R2         ; Clear R2 (R2 = 0)
SHL R0             ; R0 = R0 << 1 (multiply by 2)
SHR R3, R3         ; R3 = R3 >> 1 (divide by 2)
```

#### Jump Instructions (0x08-0x0E)

| Mnemonic | Opcode | Syntax | Description | Condition |
|----------|--------|--------|-------------|-----------|
| JMP      | 0x08   | JMP label | Jump unconditional | Always |
| JZ       | 0x09   | JZ label  | Jump if Zero | ZF = 1 |
| JNZ      | 0x0A   | JNZ label | Jump if Not Zero | ZF = 0 |
| JC       | 0x0B   | JC label  | Jump if Carry | CF = 1 |
| JNC      | 0x0C   | JNC label | Jump if Not Carry | CF = 0 |
| JN       | 0x0D   | JN label  | Jump if Negative | NF = 1 |
| JNN      | 0x0E   | JNN label | Jump if Not Negative | NF = 0 |

**Examples:**
```asm
loop:
    ADD R0, #1      ; Increment R0
    JNZ loop        ; Jump if R0 is not zero
    JMP end         ; Jump to end label

end:
    HALT            ; Stop
```

#### Special Instructions

| Mnemonic | Opcode | Syntax | Description |
|----------|--------|--------|-------------|
| CMP      | 0x0F   | CMP Rd, Rs/imm | Compare (performs subtraction and sets flags) |
| HALT     | 0xFF   | HALT | Halt CPU execution |

**Examples:**
```asm
CMP R0, #5         ; Compare R0 with 5, set flags
JZ zero_label      ; Jump if R0 equals 5
```

### Flag Bits (Set by arithmetic/compare operations)

- **ZF** (Zero Flag): Set if result is 0
- **CF** (Carry Flag): Set if overflow occurred
- **NF** (Negative Flag): Set if result is negative (bit 7 = 1)

### Assembly Syntax Rules

1. **Labels**: Define with name followed by colon (e.g., `start:`, `loop:`)
2. **Comments**: Use semicolon (`;`) for inline or line comments
3. **Case-insensitive**: Mnemonics and register names are case-insensitive
4. **Immediates**: Use `#` prefix for decimal (e.g., `#42`) or `#0x` for hex (e.g., `#0xFF`)

### Practical Examples

**Example 1: Simple Counter**
```asm
start:
    MOV R0, #0      ; Initialize counter
    MOV R1, #10     ; Load limit
loop:
    ADD R0, #1      ; Increment counter
    CMP R0, R1      ; Compare with limit
    JNZ loop        ; Continue if not equal
    HALT
```

**Example 2: Bit Shifting (LED Knight Rider)**
```asm
    MOV R0, #1      ; Start with LED0 on
shift_left:
    SHL R0          ; Shift left
    SHL R0
    SHL R0
    SHL R0          ; Now LED4 is on
shift_right:
    SHR R0          ; Shift right
    SHR R0
    SHR R0
    SHR R0          ; Back to LED0
    JMP shift_left  ; Repeat
```

## License

**BSD 3-Clause License**
<br/>Copylefts 2025
<br/>Janusz Wolak
<br/>No rights reserved

