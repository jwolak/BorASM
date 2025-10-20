#pragma once

#include <optional>
#include <string>

namespace cmd {
    struct CmdArguments {
        std::optional<std::string> input_file_path;
        std::optional<std::string> output_file_path;
    };
}  // namespace cmd