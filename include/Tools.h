#pragma once

#include <cstring>
#include <string>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

namespace tools {
    void PrintGreenOKMessage(const std::string& message);
    void PrintRedErrorMessage(const std::string& message);
    void PrintYellowWarningMessage(const std::string& message);
    void PrintGreenAssemblingMessage(const std::string& message);
    void PrintRedAssemblingMessage(const std::string& message);
    void PrintWithGreenMarker(const std::string& marker, const std::string& message);
}  // namespace tools