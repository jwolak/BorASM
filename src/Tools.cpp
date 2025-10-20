#include "Tools.h"

namespace tools {
    void PrintGreenOKMessage(const std::string& message) { std::cout << "\033[0;32m[OK]\033[0m " << message << std::endl; }
    void PrintRedErrorMessage(const std::string& message) { std::cerr << "\033[0;31m[ERROR]\033[0m " << message << std::endl; }
    void PrintYellowWarningMessage(const std::string& message) { std::cerr << "\033[0;33m[WARNING]\033[0m " << message << std::endl; }
}  // namespace tools