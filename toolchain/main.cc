#include <common/command_line.h>

#include <iostream>
#include <stdexcept>

namespace cli = viper::common::cli;

auto main(int argc, char** argv) -> int
{
    cli::CommandLine parser {};
    auto& lex_command = parser.addCommand("lex");
    
    auto file_promise = lex_command.addRequired<std::string>("--file", "-f", "The input file to tokenize");
    auto output_promise = lex_command.addRequired<std::string>("--output", "-o", "The output of the tokenized file");


    try {
        parser.parseCommands(argc, argv);
    } catch (std::runtime_error& err)
    {
        std::cout << err.what() << '\n';
        std::cout << parser.getUsageString() << '\n';
    }

    return 0;
}
