#include "BorAsm.h"

#include <memory>

#include "CmdArguments.h"
#include "Tools.h"
#include "EquinoxLogger.hpp"

namespace borasm {
    BorAsm::BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments)
        : cmd_arguments_(cmd_arguments), assembly_engine_(std::make_unique<assembly_engine::AssemblyEngine>()) {}

    /* For testing purposes only */
    BorAsm::BorAsm(std::shared_ptr<cmd::CmdArguments> cmd_arguments, std::unique_ptr<assembly_engine::IAssemblyEngine> assembly_engine)
        : cmd_arguments_(cmd_arguments), assembly_engine_(std::move(assembly_engine)) {}

    bool BorAsm::StartProcessing() {
        LOG_TRACE("[BorAsm] StartProcessing() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        tools::PrintGreenOKMessage("Starting assembly process...");
        LOG_DEBUG("[BorAsm] Starting assembly process for input file: " + cmd_arguments_->input_file_path.value() + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (!assembly_engine_->Assemble(cmd_arguments_->input_file_path.value(), cmd_arguments_->output_file_path.value())) {
            tools::PrintRedErrorMessage("Assembly process failed.");
            LOG_ERROR("[BorAsm] Assembly process failed [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return false;
        }

        tools::PrintGreenOKMessage("Assembly process completed successfully.");
        LOG_TRACE("[BorAsm] Assembly process completed successfully [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_TRACE("[BorAsm] PrintMachineCode() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        assembly_engine_->PrintMachineCode();

        LOG_TRACE("[BorAsm] SaveMachineCodeToFile() called with output_file: " + cmd_arguments_->output_file_path.value() + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (!assembly_engine_->SaveMachineCodeToFile(cmd_arguments_->output_file_path.value())) {
            tools::PrintRedErrorMessage("Failed to save machine code to file.");
            LOG_ERROR("[BorAsm] Failed to save machine code to file: " + cmd_arguments_->output_file_path.value() + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            return false;
        }
        tools::PrintGreenOKMessage("Machine code saved to file successfully.");
        LOG_TRACE("[BorAsm] Machine code saved to file successfully [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_TRACE("[BorAsm] StartProcessing() completed successfully [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        tools::PrintGreenOKMessage("Assembly processing finished successfully.");
        return true;
    }
}  // namespace borasm