#ifndef VIPER_TOOLCHAIN_DRIVER_TEST_SUBCOMMAND_H
#define VIPER_TOOLCHAIN_DRIVER_TEST_SUBCOMMAND_H

#include "common/command_line.h"
#include "common/filesystem.h"
#include "common/memory_buffer.h"
#include "common/utf-8.h"
#include "diagnostics/consumer.h"
#include "driver_subcommand.h"
#include "source/source_buffer.h"
#include <istream>
#include "testing/manager.h"
#include <memory>

namespace viper::toolchain::driver
{
    struct TestCommandOptions
    {
        // The input stream for the file to lex
        std::istream input;

        // The output stream for the tokenized output
        std::ostream output;
    };

    class TestCommand : public DriverSubcommand
    {
        // Constructors
        public:
            [[nodiscard]] explicit TestCommand() = default;

            TestCommand(const TestCommand&) = default;
            TestCommand(TestCommand&&) = default;
            ~TestCommand() override = default;

            auto operator=(const TestCommand&) -> TestCommand& = delete;
            auto operator=(TestCommand&&) -> TestCommand& = delete;

        // Info
        public:
            // Used to determine whether  
            static constexpr const char* NoOutputStreamSpecified = "__viper_test_subcommand_no_output_file_specified";

            // Name for this command
            static constexpr const char* CommandName = "test";
        
        // API
        public:
            // Build the lex subcommand
            auto build(cli::Command& command) noexcept -> void override;

            // Run the lex subcommand
            auto run() noexcept -> void override;

            // Get the name of this command
            auto name() const noexcept -> const std::string& override { return _name; }

        // Methods
        private:
            auto createLexTests() noexcept -> void;
        
        // Member fields
        private:
            // To be able to get the command name
            std::string _name { CommandName };

            testing::Manager _manager {};
            
            // Arg for test output stream name. This should either be a file or nothing.
            // If nothing, then the output goes to stdout
            cli::ArgumentPromise<std::string> _output_arg {};

    };
} // namespace viper::toolchain::driver

#endif // VIPER_TOOLCHAIN_DRIVER_TEST_SUBCOMMAND_H
