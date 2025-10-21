#include "BorAsm.h"

#include <memory>

#include "CmdArguments.h"
#include "Tools.h"
#include "spdlog/spdlog.h"

namespace borasm {
    BorAsm::BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments)
        : cmd_arguments_(cmd_arguments), assembly_engine_(std::make_unique<assembly_engine::AssemblyEngine>()) {}

    /* For testing purposes only */
    BorAsm::BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments, std::unique_ptr<assembly_engine::AssemblyEngine> assembly_engine)
        : cmd_arguments_(cmd_arguments), assembly_engine_(std::move(assembly_engine)) {}

    bool BorAsm::StartProcessing() {
        spdlog::trace("[BorAsm] StartProcessing() called [{0}:{1}]", __FILENAME__, __LINE__);

        tools::PrintGreenOKMessage("Starting assembly process...");
        spdlog::debug("[BorAsm] Starting assembly process for input file: {0} [{1}:{2}]", cmd_arguments_->input_file_path.value(), __FILENAME__, __LINE__);
        if (!assembly_engine_->Assemble(cmd_arguments_->input_file_path.value(), cmd_arguments_->output_file_path.value())) {
            tools::PrintRedErrorMessage("Assembly process failed.");
            spdlog::error("[BorAsm] Assembly process failed [{0}:{1}]", __FILENAME__, __LINE__);
            return false;
        }

        tools::PrintGreenOKMessage("Assembly process completed successfully.");
        spdlog::trace("[BorAsm] Assembly process completed successfully [{0}:{1}]", __FILENAME__, __LINE__);

        spdlog::trace("[BorAsm] PrintMachineCode() called [{0}:{1}]", __FILENAME__, __LINE__);
        assembly_engine_->PrintMachineCode();

        spdlog::trace("[BorAsm] SaveMachineCodeToFile() called with output_file: {0} [{1}:{2}]", cmd_arguments_->output_file_path.value(), __FILENAME__,
                      __LINE__);
        if (!assembly_engine_->SaveMachineCodeToFile(cmd_arguments_->output_file_path.value())) {
            tools::PrintRedErrorMessage("Failed to save machine code to file.");
            spdlog::error("[BorAsm] Failed to save machine code to file: {0} [{1}:{2}]", cmd_arguments_->output_file_path.value(), __FILENAME__, __LINE__);
            return false;
        }
        tools::PrintGreenOKMessage("Machine code saved to file successfully.");
        spdlog::trace("[BorAsm] Machine code saved to file successfully [{0}:{1}]", __FILENAME__, __LINE__);

        spdlog::trace("[BorAsm] StartProcessing() completed successfully [{0}:{1}]", __FILENAME__, __LINE__);
        tools::PrintGreenOKMessage("Assembly processing finished successfully.");
        return true;
    }
}  // namespace borasm