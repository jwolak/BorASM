#pragma once

#include <string>

namespace assembly_engine {
    class IAssemblyEngine {
      public:
        virtual ~IAssemblyEngine() = default;
        virtual bool Assemble(const std::string& input_file, const std::string& output_file) = 0;
        virtual void PrintMachineCode() const = 0;
        virtual bool SaveMachineCodeToFile(const std::string& output_file) const = 0;
    };
}  // namespace assembly_engine