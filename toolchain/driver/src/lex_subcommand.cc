#include "lex_subcommand.h"
#include <iostream>
#include <common/command_line.h>

namespace viper::toolchain::driver
{
    auto LexCommand::build(cli::Command& command) noexcept -> void
    {
        // Argument structure for the input file to lex
        _input_arg = command.addRequired<std::string>("file", "The file to tokenize");

        // Argument structure to determine the output stream
        _output_arg = command.addOptional<std::string>("output", "Where to send the tokenized output", NoOutputStreamSpecified);
    }

    auto LexCommand::run() noexcept -> void
    {
        // Something went wrong with the parsing
        if (!_input_arg.get() || !_output_arg.get())
        {
            return;
        }

        auto input_name = _input_arg.get().value();
        auto output_name = _output_arg.get().value();
        
        std::cout << "Input file: " << input_name << '\n';
        std::cout << "Output file: " << output_name << '\n';
    }
} // namespace viper::toolchain::driver
