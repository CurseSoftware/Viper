#include "lex/lex.h"
#include "lex_subcommand.h"
#include "common/filesystem.h"
#include "common/memory_buffer.h"
#include "common/utf-8.h"
#include <iostream>
#include <common/command_line.h>
#include <base/log.h>

#include <filesystem>
#include <optional>

namespace viper::toolchain::driver
{
    namespace log = base::log;
    
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

        log::info("Input file: {}", input_name);
        log::info("Output file: {}", output_name);
        
        auto source_buffer = createSourceBuffer();
        if (!source_buffer)
        {
            std::cout << "Failed to create source buffer\n";
        }

        source_buffer->printAsAscii();
        std::cout << '\n';

        lex::lex();
    }

    auto LexCommand::createSourceMemoryBuffer() noexcept -> std::unique_ptr<memory::MemoryBuffer>
    {
        if (!_input_arg.get())
        {
            std::cout << "Invalid input argument\n";
            return nullptr;
        }
        std::cout << "Reading in file: " << _input_arg.get().value() << '\n';

        auto file_opt = fs::File::fromPath(_input_arg.get().value());
        if (!file_opt)
        {
            std::cout << "Failed to create file\n";
            return nullptr;
        }
        std::cout << "File created.\n";

        auto file = std::move(file_opt.value());
        auto content = file.readContentToBytes();
        std::cout << "Content buffer size: " << content.size() << '\n';
        auto memory_buffer = memory::MemoryBuffer::from(content.data(), content.size());
        if (!memory_buffer)
        {
            std::cout << "Failed to create memory buffer\n"; return nullptr;
        }
        std::cout << "Memory buffer created\n";
        
        return std::move(memory_buffer.value());
    }

    auto LexCommand::createSourceBuffer() noexcept -> std::optional<source::SourceBuffer>
    {
        if (!_input_arg.get())
        {
            std::cout << "Invalid input argument\n";
            return std::nullopt;
        }
        std::cout << "Creating source buffer from: " << _input_arg.get().value() << '\n';

        auto source_buffer_opt = source::SourceBuffer::fromFilePath(_input_arg.get().value());

        return source_buffer_opt;
    }
} // namespace viper::toolchain::driver
