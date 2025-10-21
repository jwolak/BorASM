#pragma once

#include <memory>

#include "AssemblyEngine.h"
#include "CmdArguments.h"

namespace borasm {
    class BorAsm {
      public:
        explicit BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments);
        bool StartProcessing();

      protected:
        BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments, std::unique_ptr<assembly_engine::IAssemblyEngine> assembly_engine);

      private:
        std::shared_ptr<cmd::CmdArguments> cmd_arguments_;
        std::unique_ptr<assembly_engine::IAssemblyEngine> assembly_engine_;
    };
}  // namespace borasm