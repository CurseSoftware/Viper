#include <common/command_line.h>

#include <iostream>
#include <stdexcept>

namespace cli = viper::common::cli;

auto main(int argc, char** argv) -> int
{
    cli::ArgumentParser parser {};
    auto& lex_command = parser.addCommand("lex", "description");
    lex_command.addRequired<std::string>("--file", "-f", "The input file to tokenize");
    lex_command.addRequired<std::string>("--output", "-o", "The output of the tokenized file");
    lex_command.addRequired<int>("--integral", "-i", "Test integral parsing");
    lex_command.addRequired<float>("--floating", "-x", "Test floating point parsing");

    std::cout << parser.getUsageString() << '\n';

    try {
        parser.parseCommands(argc, argv);
    } catch (std::runtime_error& err)
    {
        std::cout << err.what() << '\n';
    }

    return 0;
}
