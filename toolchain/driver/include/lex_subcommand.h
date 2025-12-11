#ifndef VIPER_TOOLCHAIN_DRIVER_LEX_SUBCOMMAND_H
#define VIPER_TOOLCHAIN_DRIVER_LEX_SUBCOMMAND_H

#include "common/command_line.h"
#include "driver_subcommand.h"
#include <istream>

namespace viper::toolchain::driver
{
    namespace cli = common::cli;
    
    struct LexCommandOptions
    {
        std::istream input;
        std::ostream output;
    };

    class LexCommand : public DriverSubcommand
    {
        public:
            [[nodiscard]] explicit LexCommand() = default;
            LexCommand(const LexCommand&) = default;
            LexCommand(LexCommand&&) = default;
            ~LexCommand() override = default;

            auto operator=(const LexCommand&) -> LexCommand& = default;
            auto operator=(LexCommand&&) -> LexCommand& = default;

        public:
            static constexpr const char* NoOutputStreamSpecified = "__lex_subcommand_no_output_file_specified";

            static constexpr const char* CommandName = "lex";
        
        public:
            // Build the lex subcommand
            auto build(cli::Command& command) noexcept -> void override;

            auto run() noexcept -> void override;

            auto name() const noexcept -> const std::string& override { return _name; }
        
        private:
            // To be able to get the command name
            std::string _name { CommandName };
            
            // Promises of values parsed from the command lines
            
            // Arg for input to tokenize. This should always point to a file
            cli::ArgumentPromise<std::string> _input_arg  {};

            // Arg for output to tokenize. This should either be a file or nothing.
            // If nothing, then the output goes to stdout
            cli::ArgumentPromise<std::string> _output_arg {};

    };
} // namespace viper::toolchain::driver

#endif // VIPER_TOOLCHAIN_DRIVER_LEX_SUBCOMMAND_H
