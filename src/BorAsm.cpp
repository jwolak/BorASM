#include "BorAsm.h"

#include <memory>

#include "CmdArguments.h"

namespace borasm {
    BorAsm::BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments) : cmd_arguments_(cmd_arguments) {}
}  // namespace borasm