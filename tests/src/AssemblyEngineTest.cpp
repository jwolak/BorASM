#include <gtest/gtest.h>

#include "AssemblyEngine.h"
#include "CharacterStringLineHandlerMock.h"
#include "FileHandlerMock.h"

namespace assembly_engine_test {

    class AssemblyEngineWithInjectedMocks : public assembly_engine::AssemblyEngine {
      public:
        AssemblyEngineWithInjectedMocks(std::unique_ptr<assembly_engine::IFileHandler> file_handler,
                                        std::unique_ptr<assembly_engine::ICharacterStringLineHandler> line_handler)
            : AssemblyEngine(std::move(file_handler), std::move(line_handler)) {}
    };

    class AssemblyEngineTest : public ::testing::Test {
      public:
        AssemblyEngineTest()
            : file_handler_mock{new mocks::FileHandlerMock()},
              character_string_line_handler_mock{new mocks::CharacterStringLineHandlerMock()},
              assembly_engine_with_injected_mocks(std::unique_ptr<assembly_engine::IFileHandler>(file_handler_mock),
                                                  std::unique_ptr<assembly_engine::ICharacterStringLineHandler>(character_string_line_handler_mock)) {}

        mocks::FileHandlerMock* file_handler_mock;
        mocks::CharacterStringLineHandlerMock* character_string_line_handler_mock;
        AssemblyEngineWithInjectedMocks assembly_engine_with_injected_mocks;
    };

}  // namespace assembly_engine_test