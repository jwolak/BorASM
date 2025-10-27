# BorASM
Assembly compiler for Boruss CPU

```
 ____   ___  ____      _    ____  __  __ 
| __ ) / _ \|  _ \    / \  / ___||  \/  |
|  _ \| | | | |_) |  / _ \ \___ \| |\/| |
| |_) | |_| |  _ <  / ___ \ ___) | |  | |
|____/ \___/|_| \_\/_/   \_\____/|_|  |_|

Boruss CPU Assembler v1.0 
Target: "Laibach" Core (8-bit RISC)
```

A small educational assembler written in C++ for a custom toy CPU architecture. BorASM reads assembly source files, detects labels and references, and emits a compact hexadecimal machine-code representation. The project includes a small instruction set, unit tests (Google Test / Google Mock), and utilities for parsing and assembling source files.

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

