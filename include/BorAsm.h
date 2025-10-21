#pragma once

#include <memory>

#include "AssemblyEngine.h"
#include "CmdArguments.h"

namespace borasm {
    class BorAsm {
      public:
        explicit BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments);
        bool StartProcessing();
        void PrintMachineCode() const;

      private:
        std::shared_ptr<cmd::CmdArguments> cmd_arguments_;
        std::unique_ptr<assembly_engine::AssemblyEngine> assembly_engine_;
    };
}  // namespace borasm