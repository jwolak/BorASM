#include <gtest/gtest.h>

#include "AssemblyEngineMock.h"
#include "BorAsm.h"

namespace borasm_test {

    class BorAsmWithInjectedAssemblyEngine : public borasm::BorAsm {
      public:
        BorAsmWithInjectedAssemblyEngine(std::shared_ptr<cmd::CmdArguments> cmd_arguments, std::unique_ptr<assembly_engine::IAssemblyEngine> assembly_engine)
            : borasm::BorAsm(cmd_arguments, std::move(assembly_engine)) {}
    };

    class BorAsmTest : public ::testing::Test {
      public:
        BorAsmTest()
            : assembly_engine_mock{new mocks::AssemblyEngineMock()},
              cmd_arguments{std::make_shared<cmd::CmdArguments>()},
              borasm_with_injected_assembly_engine{cmd_arguments, std::unique_ptr<assembly_engine::IAssemblyEngine>(assembly_engine_mock)} {}

        void SetUp() override {}
        void TearDown() override {}

        mocks::AssemblyEngineMock* assembly_engine_mock;
        std::shared_ptr<cmd::CmdArguments> cmd_arguments;
        BorAsmWithInjectedAssemblyEngine borasm_with_injected_assembly_engine;
    };

    TEST_F(BorAsmTest, Try_To_Start_Processing_But_Assembly_Fails_And_False_Returned) { ASSERT_FALSE(borasm_with_injected_assembly_engine.StartProcessing()); }

    TEST_F(BorAsmTest, Try_To_Start_Processing_But_Save_Machine_Code_To_File_Fails_And_False_Returned) {
        ASSERT_FALSE(borasm_with_injected_assembly_engine.StartProcessing());
    }

    TEST_F(BorAsmTest, Start_Processing_Successfully_And_Returns_True) { ASSERT_TRUE(borasm_with_injected_assembly_engine.StartProcessing()); }

}  // namespace borasm_test