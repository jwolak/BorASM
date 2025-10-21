#include "BorAsm.h"

#include <memory>

#include "CmdArguments.h"
#include "Tools.h"
#include "spdlog/spdlog.h"

namespace borasm {
    BorAsm::BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments)
        : cmd_arguments_(cmd_arguments), assembly_engine_(std::make_unique<assembly_engine::AssemblyEngine>()) {}

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
        return true;
    }

    void BorAsm::PrintMachineCode() const {
        spdlog::trace("[BorAsm] PrintMachineCode() called [{0}:{1}]", __FILENAME__, __LINE__);
        assembly_engine_->PrintMachineCode();
    }

    void BorAsm::SaveMachineCodeToFile(const std::string& output_file) const {
        spdlog::trace("[BorAsm] SaveMachineCodeToFile() called with output_file: {0} [{1}:{2}]", output_file, __FILENAME__, __LINE__);
        assembly_engine_->SaveMachineCodeToFile(output_file);
    }

}  // namespace borasm