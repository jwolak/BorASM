#pragma once

#include <gmock/gmock.h>

#include "IInstructionsAssemblerCore.h"

namespace mocks {
    class InstructionsAssemblerCoreMock : public assembly_engine::IInstructionsAssemblerCore {
      public:
        MOCK_METHOD(bool, AssembleInstruction, (const std::vector<std::string>& tokens), (override));
    };
}  // namespace mocks