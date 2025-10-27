#include <gtest/gtest.h>

#include "AssemblyEngine.h"
#include "CodeAnalyzerMock.h"
#include "FileHandlerMock.h"

namespace assembly_engine_test {

    using namespace testing;

    namespace {
        constexpr const char* kNonexistentInputFile = "nonexistent_input.asm";
        constexpr const char* kInvalidInputFile = "invalid_input.asm";
        constexpr const char* kValidInputFile = "valid_input.asm";
        constexpr const char* kOutputFile = "output.bin";
    }  // namespace

    class AssemblyEngineWithInjectedMocks : public assembly_engine::AssemblyEngine {
      public:
        AssemblyEngineWithInjectedMocks(std::unique_ptr<assembly_engine::IFileHandler> file_handler,
                                        std::unique_ptr<assembly_engine::ICodeAnalyzer> code_analyzer, std::vector<uint8_t> machine_code)
            : AssemblyEngine(std::move(file_handler), std::move(code_analyzer), machine_code) {}
    };

    class AssemblyEngineTest : public ::testing::Test {
      public:
        AssemblyEngineTest()
            : file_handler_mock{new mocks::FileHandlerMock()},
              code_analyzer_mock{new mocks::CodeAnalyzerMock()},
              machine_code{{0x01, 0xAB, 0xFF, 0x10}},
              assembly_engine_with_injected_mocks(std::unique_ptr<assembly_engine::IFileHandler>(file_handler_mock),
                                                  std::unique_ptr<assembly_engine::ICodeAnalyzer>(code_analyzer_mock), machine_code),
              dummy_input_stream{} {}

        mocks::FileHandlerMock* file_handler_mock;
        mocks::CodeAnalyzerMock* code_analyzer_mock;
        std::vector<uint8_t> machine_code;
        AssemblyEngineWithInjectedMocks assembly_engine_with_injected_mocks;
        std::ifstream dummy_input_stream;
    };

    TEST_F(AssemblyEngineTest, Try_Assemble_File_Nonexistent_Input_File_And_Returns_False) {
        EXPECT_CALL(*file_handler_mock, OpenFileToRead(_)).WillOnce(::testing::Return(false));
        EXPECT_CALL(*file_handler_mock, GetFileToReadStream()).Times(0);
        EXPECT_CALL(*code_analyzer_mock, DetectLabels(_, _)).Times(0);
        EXPECT_CALL(*code_analyzer_mock, Tokenize(_, _)).Times(0);
        EXPECT_CALL(*code_analyzer_mock, ResolveLabelReferences()).Times(0);

        EXPECT_FALSE(assembly_engine_with_injected_mocks.Assemble(kNonexistentInputFile, kOutputFile));
    }

    TEST_F(AssemblyEngineTest, Try_Assemble_File_But_Detect_Lababels_Failed_And_Returns_False) {
        EXPECT_CALL(*file_handler_mock, OpenFileToRead(_)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*file_handler_mock, GetFileToReadStream()).WillOnce(::testing::ReturnRef(dummy_input_stream));
        EXPECT_CALL(*code_analyzer_mock, DetectLabels(_, _)).WillOnce(::testing::Return(false));
        EXPECT_CALL(*code_analyzer_mock, Tokenize(_, _)).Times(0);
        EXPECT_CALL(*code_analyzer_mock, ResolveLabelReferences()).Times(0);

        EXPECT_FALSE(assembly_engine_with_injected_mocks.Assemble(kValidInputFile, kOutputFile));
    }

    TEST_F(AssemblyEngineTest, Try_Assemble_File_But_Tokenize_Failed_And_Returns_False) {
        EXPECT_CALL(*file_handler_mock, OpenFileToRead(_)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*file_handler_mock, GetFileToReadStream()).WillOnce(::testing::ReturnRef(dummy_input_stream));
        EXPECT_CALL(*code_analyzer_mock, DetectLabels(_, _)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*code_analyzer_mock, Tokenize(_, _)).WillOnce(::testing::Return(false));
        EXPECT_CALL(*code_analyzer_mock, ResolveLabelReferences()).Times(0);

        EXPECT_FALSE(assembly_engine_with_injected_mocks.Assemble(kValidInputFile, kOutputFile));
    }

    TEST_F(AssemblyEngineTest, Try_Assemble_File_But_Resolve_Labels_References_Failed_And_Returns_False) {
        EXPECT_CALL(*file_handler_mock, OpenFileToRead(_)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*file_handler_mock, GetFileToReadStream()).WillOnce(::testing::ReturnRef(dummy_input_stream));
        EXPECT_CALL(*code_analyzer_mock, DetectLabels(_, _)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*code_analyzer_mock, Tokenize(_, _)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*code_analyzer_mock, ResolveLabelReferences()).WillOnce(::testing::Return(false));

        EXPECT_FALSE(assembly_engine_with_injected_mocks.Assemble(kValidInputFile, kOutputFile));
    }

    TEST_F(AssemblyEngineTest, Assemble_File_Successfully_And_Returns_True) {
        EXPECT_CALL(*file_handler_mock, OpenFileToRead(_)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*file_handler_mock, GetFileToReadStream()).WillOnce(::testing::ReturnRef(dummy_input_stream));
        EXPECT_CALL(*code_analyzer_mock, DetectLabels(_, _)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*code_analyzer_mock, Tokenize(_, _)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*code_analyzer_mock, ResolveLabelReferences()).WillOnce(::testing::Return(true));

        EXPECT_TRUE(assembly_engine_with_injected_mocks.Assemble(kValidInputFile, kOutputFile));
    }

    TEST_F(AssemblyEngineTest, Print_Machine_Code_And_Expected_Output_Code_Printed) {
        // Capture std::cout
        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        assembly_engine_with_injected_mocks.PrintMachineCode();
        std::cout.rdbuf(old);
        std::string output = buffer.str();

        EXPECT_NE(output.find("01 AB FF 10"), std::string::npos);
    }

    TEST_F(AssemblyEngineTest, Save_Machine_Code_And_Expected_Output_Code_Saved_In_File) {
        std::ofstream saved_file(kOutputFile, std::ios::out | std::ios::binary);
        EXPECT_CALL(*file_handler_mock, OpenFileToWrite(_)).WillOnce(::testing::Return(true));
        EXPECT_CALL(*file_handler_mock, GetFileToWriteStream()).WillOnce(::testing::ReturnRef(saved_file));
        assembly_engine_with_injected_mocks.SaveMachineCodeToFile(kOutputFile);

        // Read the contents of the file
        std::ifstream read_save_file(kOutputFile);
        std::stringstream buffer;
        buffer << read_save_file.rdbuf();
        std::string output = buffer.str();

        EXPECT_EQ(output, "01\nAB\nFF\n10\n");

        read_save_file.close();
        std::remove(kOutputFile);
    }

}  // namespace assembly_engine_test