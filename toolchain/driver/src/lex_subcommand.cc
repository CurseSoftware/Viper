#include "lex_subcommand.h"

namespace viper::toolchain::driver
{
    namespace cli = common::cli;

    auto LexCommand::build(cli::Command& command) noexcept -> void
    {
        _input_arg = command.addRequired<std::string>("file", "The file to tokenize");
        _output_arg = command.addOptional<std::string>("output", "Where to send the tokenized output", NoOutputStreamSpecified);
    }

    auto LexCommand::run() noexcept -> void
    {
    }
} // namespace viper::toolchain::driver
