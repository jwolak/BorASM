#pragma once

#include <gmock/gmock.h>

#include "IAssemblyEngine.h"

namespace mocks {
    class AssemblyEngineMock : public assembly_engine::IAssemblyEngine {
      public:
        MOCK_METHOD(bool, Assemble, (const std::string& input_file, const std::string& output_file), (override));
        MOCK_METHOD(void, PrintMachineCode, (), (const, override));
        MOCK_METHOD(bool, SaveMachineCodeToFile, (const std::string& output_file), (const, override));
    };
}  // namespace mocks