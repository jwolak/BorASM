#include <gtest/gtest.h>

#include "AssemblyEngine.h"
#include "CharacterStringLineHandlerMock.h"
#include "FileHandlerMock.h"
#include "InstructionsAssemblerCoreMock.h"

namespace assembly_engine_test {

    namespace {
        constexpr const char* kNonexistentInputFile = "nonexistent_input.asm";
        constexpr const char* kInvalidInputFile = "invalid_input.asm";
        constexpr const char* kOutputFile = "output.bin";
    }  // namespace

    class AssemblyEngineWithInjectedMocks : public assembly_engine::AssemblyEngine {
      public:
        AssemblyEngineWithInjectedMocks(std::unique_ptr<assembly_engine::IFileHandler> file_handler) : AssemblyEngine(std::move(file_handler)) {}
    };

    class AssemblyEngineTest : public ::testing::Test {
      public:
        AssemblyEngineTest()
            : file_handler_mock{new mocks::FileHandlerMock()},
              assembly_engine_with_injected_mocks(std::unique_ptr<assembly_engine::IFileHandler>(file_handler_mock)) {}

        mocks::FileHandlerMock* file_handler_mock;
        AssemblyEngineWithInjectedMocks assembly_engine_with_injected_mocks;
    };

    TEST_F(AssemblyEngineTest, Try_Assemble_File_Nonexistent_Input_File_And_Returns_False) {
        EXPECT_CALL(*file_handler_mock, OpenFileToRead(kNonexistentInputFile)).WillOnce(::testing::Return(false));

        EXPECT_FALSE(assembly_engine_with_injected_mocks.Assemble(kNonexistentInputFile, kOutputFile));
    }

}  // namespace assembly_engine_test