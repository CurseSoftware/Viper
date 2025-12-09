#include <common/command_line.h>

#include <iostream>

namespace cli = viper::common::cli;

auto main(int argc, char** argv) -> int
{
    (void) argc;
    (void) argv;

    cli::ArgumentParser parser {};
    auto& lex_command = parser.addCommand("lex", "description");
    lex_command.addRequired<std::string>("--file", "-f", "The input file to tokenize");

    std::cout << "usage: " << parser.getUsageString() << '\n';

    return 0;
}
