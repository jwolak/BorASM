#include <gtest/gtest.h>

#include "FileHandler.h"

namespace file_handler_test {

    namespace {
        constexpr const char* kInputFilePath = "test_input.asm";
        constexpr const char* kOutputFilePath = "test_output.bin";
        constexpr const char* kNonExistentFilePath = "non_existent_file.bin";
    }  // namespace

    class FileHandlerTest : public ::testing::Test {
      public:
        FileHandlerTest() : file_handler{} {}

        void SetUp() override {
            std::ofstream ofs(kInputFilePath);
            ofs << "; Test input file for FileHandlerTests\nMOV AX, BX\nADD AX, 1\n";
            ofs.close();
        }
        void TearDown() override {
            std::remove(kNonExistentFilePath);
            std::remove(kInputFilePath);
            std::remove(kOutputFilePath);
        }

        assembly_engine::FileHandler file_handler;
    };

    TEST_F(FileHandlerTest, Try_Open_File_To_Read_But_File_Does_Not_Exist_And_Returns_False) {
        ASSERT_FALSE(file_handler.OpenFileToRead(kNonExistentFilePath));
    }

    TEST_F(FileHandlerTest, Open_File_To_Read_Successful_And_Returns_True) { ASSERT_TRUE(file_handler.OpenFileToRead(kInputFilePath)); }

    TEST_F(FileHandlerTest, Try_Open_File_To_Write_But_File_Does_Not_Exist_And_Is_Created_Then_Returns_True) {
        ASSERT_TRUE(file_handler.OpenFileToWrite(kNonExistentFilePath));
    }

    TEST_F(FileHandlerTest, Open_File_To_Write_Successful_And_Returns_True) { ASSERT_TRUE(file_handler.OpenFileToWrite(kOutputFilePath)); }

}  // namespace file_handler_test