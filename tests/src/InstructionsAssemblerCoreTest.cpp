#include <gtest/gtest.h>

#include "CharacterStringLineHandlerMock.h"
#include "InstructionsAssemblerCore.h"

namespace instructions_assembler_core_test {

    class InstructionsAssemblerCoreWithInjectedMocks : public assembly_engine::InstructionsAssemblerCore {
      public:
        InstructionsAssemblerCoreWithInjectedMocks(std::vector<uint8_t>& machine_code, std::vector<std::pair<int, std::string>>& label_references,
                                                   std::unique_ptr<assembly_engine::ICharacterStringLineHandler> line_handler)
            : InstructionsAssemblerCore(machine_code, label_references, std::move(line_handler)) {}
    };

    class InstructionsAssemblerCoreTest : public ::testing::Test {
      public:
        InstructionsAssemblerCoreTest()
            : machine_code{},
              label_references{},
              character_string_line_handler_mock{new mocks::CharacterStringLineHandlerMock},
              instructions_assembler_core_with_injected_mocks{
                  machine_code, label_references, std::unique_ptr<assembly_engine::ICharacterStringLineHandler>(character_string_line_handler_mock)} {}

        std::vector<uint8_t> machine_code;
        std::vector<std::pair<int, std::string>> label_references;
        mocks::CharacterStringLineHandlerMock* character_string_line_handler_mock;
        InstructionsAssemblerCoreWithInjectedMocks instructions_assembler_core_with_injected_mocks;
    };

}  // namespace instructions_assembler_core_test
