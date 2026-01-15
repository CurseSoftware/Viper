#include "compilation_unit.h"
#include "parse_subcommand.h"
#include "common/filesystem.h"
#include "common/memory_buffer.h"
#include <iostream>
#include <common/command_line.h>
#include <base/log.h>

#include <optional>

namespace viper::toolchain::driver
{
    namespace log = base::log;
    
    auto ParseCommand::build(cli::Command& command) noexcept -> void
    {
        // Argument structure for the input file to lex
        _input_arg = command.addRequired<std::string>("file", "The file to parse");

        // Argument structure to determine the output stream
        _output_arg = command.addOptional<std::string>("output", "Where to send the parsed output", NoOutputStreamSpecified);
    }

    auto ParseCommand::run() noexcept -> void
    {
        // Something went wrong with the parsing
        if (!_input_arg.get() || !_output_arg.get())
        {
            return;
        }

        auto input_name = _input_arg.get().value();
        auto output_name = _output_arg.get().value();

        log::info("Input file: {}", input_name);
        log::info("Output file: {}", output_name);

        auto compilation_unit = CompilationUnit(input_name, _diagnostics_consumer);
        compilation_unit.tokenize();
    }
} // namespace viper::toolchain::driver
