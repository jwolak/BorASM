#!/bin/bash
# Run cpplint on all C++ source and header files in the project

set -e

# Find all .cpp and .h files (excluding build and third-party)
find ../src ../include ../tests -type f \( -name "*.cpp" -o -name "*.h" \) | while read file; do
    if [[ "$file" == ../include/* ]]; then
        cpplint --filter=-legal/copyright,-runtime/indentation_namespace,-whitespace/ending_newline,-whitespace/indent,-build/header_guard,-build/include_subdir --linelength=160 "$file"
    else
        cpplint --filter=-legal/copyright,-runtime/indentation_namespace,-whitespace/ending_newline,-whitespace/indent,-build/header_guard,-build/include_subdir --linelength=160 "$file"
    fi
done
