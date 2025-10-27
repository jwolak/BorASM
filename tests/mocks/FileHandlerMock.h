#pragma once

#include <gmock/gmock.h>

#include "IFileHandler.h"

namespace mocks {
    class FileHandlerMock : public assembly_engine::IFileHandler {
      public:
        MOCK_METHOD(bool, OpenFileToRead, (const std::string& file_path), (override));
        MOCK_METHOD(bool, OpenFileToWrite, (const std::string& file_path), (override));
        MOCK_METHOD(std::ifstream&, GetFileToReadStream, (), (override));
        MOCK_METHOD(std::ofstream&, GetFileToWriteStream, (), (override));
    };
}  // namespace mocks