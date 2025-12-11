#include <driver.h>

#include <iostream>
#include <stdexcept>

namespace cli = viper::common::cli;
namespace toolchain = viper::toolchain;

auto main(int argc, char** argv) -> int
{
    /*
    cli::CommandLine parser {};
    auto& lex_command = parser.addCommand("lex");
    
    auto file_promise = lex_command.addRequired<std::string>("--file", "-f", "The input file to tokenize");
    auto output_promise = lex_command.addRequired<std::string>("--output", "-o", "The output of the tokenized file");


    try {
        (void) parser.parseCommands(argc, argv);
    } catch (std::runtime_error& err)
    {
        std::cout << err.what() << '\n';
        std::cout << parser.getUsageString() << '\n';
    }
    */

    toolchain::driver::Driver program_driver {};
    program_driver.run(argc, argv);

    return 0;
}
