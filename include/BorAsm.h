#pragma once

#include <memory>

#include "CmdArguments.h"

namespace borasm {
    class BorAsm {
      public:
        BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments);

      private:
        std::shared_ptr<cmd::CmdArguments> cmd_arguments_;
    };
}  // namespace borasm