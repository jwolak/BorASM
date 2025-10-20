#pragma once

#include <iostream>
#include <string>

namespace tools {
    void PrintGreenOKMessage(const std::string& message);
    void PrintRedErrorMessage(const std::string& message);
    void PrintYellowWarningMessage(const std::string& message);
}