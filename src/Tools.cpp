#include "Tools.h"

#include <fmt/format.h>

namespace tools {
    void PrintGreenOKMessage(const std::string& message) { fmt::print("\033[0;32m[OK]\033[0m {}\n", message); }
    void PrintRedErrorMessage(const std::string& message) { fmt::print(stderr, "\033[0;31m[ERROR]\033[0m {}\n", message); }
    void PrintYellowWarningMessage(const std::string& message) { fmt::print(stderr, "\033[0;33m[WARNING]\033[0m {}\n", message); }
    void PrintGreenAssemblingMessage(const std::string& message) { fmt::print("\033[0;32m[Assembling]\033[0m {}\n", message); }
    void PrintRedAssemblingMessage(const std::string& message) { fmt::print("\033[0;31m[Assembling ERROR]\033[0m {}\n", message); }
    void PrintWithGreenMarker(const std::string& marker, const std::string& message) { fmt::print("\033[0;32m[{}]\033[0m {}\n", marker, message); }
}  // namespace tools