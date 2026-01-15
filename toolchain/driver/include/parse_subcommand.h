#ifndef VIPER_TOOLCHAIN_DRIVER_PARSE_SUBCOMMAND_H
#define VIPER_TOOLCHAIN_DRIVER_PARSE_SUBCOMMAND_H

#include "common/command_line.h"
#include "common/filesystem.h"
#include "common/memory_buffer.h"
#include "common/utf-8.h"
#include "driver_subcommand.h"
#include "source/source_buffer.h"
#include <istream>
#include <memory>

namespace viper::toolchain::driver
{
    struct ParseCommandOptions
    {
        // The input stream for the file to lex
        std::istream input;

        // The output stream for the tokenized output
        std::ostream output;
    };

    class ParseCommand : public DriverSubcommand
    {
        // Constructors
        public:
            [[nodiscard]] explicit ParseCommand(std::weak_ptr<diagnostics::Consumer> consumer)
                : _diagnostics_consumer{ std::move(consumer) }
            {}

            ParseCommand(const ParseCommand&) = default;
            ParseCommand(ParseCommand&&) = default;
            ~ParseCommand() override = default;

            auto operator=(const ParseCommand&) -> ParseCommand& = default;
            auto operator=(ParseCommand&&) -> ParseCommand& = default;

        // Info
        public:
            // Used to determine whether  
            static constexpr const char* NoOutputStreamSpecified = "__viper_lex_subcommand_no_output_file_specified";

            // Name for this command
            static constexpr const char* CommandName = "lex";
        
        // API
        public:
            // Build the parse subcommand
            auto build(cli::Command& command) noexcept -> void override;

            // Run the parse subcommand
            auto run() noexcept -> void override;

            // Get the name of this command
            auto name() const noexcept -> const std::string& override { return _name; }

        // Member fields
        private:
            // To be able to get the command name
            std::string _name { CommandName };

            // Diagnostics consumer
            std::weak_ptr<diagnostics::Consumer> _diagnostics_consumer;
            
            /* Promises for the command line arguments */
            
            // Arg for input to tokenize. This should always point to a file
            cli::ArgumentPromise<std::string> _input_arg  {};

            // Arg for output to tokenize. This should either be a file or nothing.
            // If nothing, then the output goes to stdout
            cli::ArgumentPromise<std::string> _output_arg {};

    };
} // namespace viper::toolchain::driver

#endif // VIPER_TOOLCHAIN_DRIVER_PARSE_SUBCOMMAND_H
