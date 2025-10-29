# BorASM
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
51       0101 0001       MOV    1
01       0000 0001        -     1
60       0110 0000       SHL    R0
60       0110 0000       SHL    R0
60       0110 0000       SHL    R0
60       0110 0000       SHL    R0
70       0111 0000       SHR    R0
70       0111 0000       SHR    R0
70       0111 0000       SHR    R0
70       0111 0000       SHR    R0
80       1000 0000       JMP    -
02       0000 0010        -     2
```

## License

**BSD 3-Clause License**
<br/>Copylefts 2025
<br/>Janusz Wolak
<br/>No rights reserved

