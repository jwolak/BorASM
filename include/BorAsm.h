#pragma once

#include <memory>

#include "CmdArguments.h"

namespace borasm {
    class BorAsm {
      public:
        explicit BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments);
        bool StartProcessing();

      private:
        std::shared_ptr<cmd::CmdArguments> cmd_arguments_;
    };
}  // namespace borasm