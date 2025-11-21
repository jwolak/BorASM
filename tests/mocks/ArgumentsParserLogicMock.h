#pragma once

#include <gmock/gmock.h>

#include "IArgumentsParserLogic.h"

namespace mocks {
    class ArgumentsParserLogicMock : public cmd::IArgumentsParserLogic {
      public:
        MOCK_CONST_METHOD0(PrintHelp, void());
        MOCK_CONST_METHOD0(PrintVersionInfo, void());
        MOCK_CONST_METHOD0(EnableDebugMode, bool());
        MOCK_CONST_METHOD1(GetInputFileName, std::optional<std::string>(const char* optarg));
        MOCK_CONST_METHOD1(GetOutputFileName, std::optional<std::string>(const char* optarg));
        MOCK_CONST_METHOD0(ListAvailableInstructions, void());
        MOCK_CONST_METHOD1(SetInputFileAsOutputFileName, std::string(const std::string& input_file_name));
        MOCK_CONST_METHOD2(CheckOutputFileNameIsNotSameAsInputFileName, bool(const std::string& input_file_name, const std::string& output_file_name));
    };
}  // namespace mocks