#include <gtest/gtest.h>

#include "CharacterStringLineHandlerMock.h"
#include "CodeAnalyzer.h"
#include "FileHandler.h"
#include "InstructionsAssemblerCoreMock.h"

namespace code_analyzer_test {

    class CodeAnalyzerWithInjectedMocks : public assembly_engine::CodeAnalyzer {
      public:
        CodeAnalyzerWithInjectedMocks(std::vector<uint8_t>& machine_code, std::map<std::string, uint16_t>& labels,
                                      std::vector<std::pair<int, std::string>>& label_references,
                                      std::unique_ptr<assembly_engine::ICharacterStringLineHandler> line_handle,
                                      std::unique_ptr<assembly_engine::IInstructionsAssemblerCore> instructions_assembler_core)
            : CodeAnalyzer(machine_code, labels, label_references, std::move(line_handle), std::move(instructions_assembler_core)) {}
    };

    class CodeAnalyzerTest : public ::testing::Test {
      public:
        CodeAnalyzerTest()
            : machine_code_{},
              labels_{},
              label_references_{},
              line_handler_mock_{new mocks::CharacterStringLineHandlerMock},
              instructions_assembler_core_mock_{new mocks::InstructionsAssemblerCoreMock},
              code_analyzer_with_injected_mocks{machine_code_, labels_, label_references_,
                                                std::unique_ptr<assembly_engine::ICharacterStringLineHandler>(line_handler_mock_),
                                                std::unique_ptr<assembly_engine::IInstructionsAssemblerCore>(instructions_assembler_core_mock_)} {}

        std::vector<uint8_t> machine_code_;
        std::map<std::string, uint16_t> labels_;
        std::vector<std::pair<int, std::string>> label_references_;
        mocks::CharacterStringLineHandlerMock* line_handler_mock_;
        mocks::InstructionsAssemblerCoreMock* instructions_assembler_core_mock_;
        CodeAnalyzerWithInjectedMocks code_analyzer_with_injected_mocks;
    };

    // Test cases for CodeAnalyzer would go here

}  // namespace code_analyzer_test