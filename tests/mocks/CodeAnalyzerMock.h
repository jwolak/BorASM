#pragma once

#include <gmock/gmock.h>

#include <fstream>
#include <string>

#include "ICodeAnalyzer.h"

namespace mocks {
    class CodeAnalyzerMock : public assembly_engine::ICodeAnalyzer {
      public:
        MOCK_METHOD(bool, DetectLabels, (std::ifstream & file, std::string& line), (override));
        MOCK_METHOD(bool, Tokenize, (std::ifstream & file, std::string& line), (override));
        MOCK_METHOD(bool, ResolveLabelReferences, (), (override));
    };
}  // namespace mocks