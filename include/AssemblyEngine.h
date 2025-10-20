#pragma once

#include <string>

namespace assembly_engine {
    class AssemblyEngine {
      public:
        AssemblyEngine();
        bool Assemble(const std::string& input_file, const std::string& output_file);
    };
}  // namespace assembly_engine