#pragma once

#include <gmock/gmock.h>

#include "ICharacterStringLineHandler.h"

namespace mocks {
    class CharacterStringLineHandlerMock : public assembly_engine::ICharacterStringLineHandler {
      public:
        MOCK_METHOD(std::string, CleanLineWhitespaces, (const std::string& line_to_be_processed), (const, override));
        MOCK_METHOD(std::string, RemoveLineComments, (const std::string& line_to_be_processed), (const, override));
        MOCK_METHOD(bool, IsNumber, (const std::string& string_number), (const, override));
        MOCK_METHOD(uint8_t, ConvertStringToNumber, (const std::string& string_number), (const, override));
        MOCK_METHOD(std::vector<std::string>, TokenizeLine, (const std::string& line_to_be_processed), (const, override));
    };
}  // namespace mocks