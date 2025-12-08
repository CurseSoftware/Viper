#include <iostream>
#include <common/command_line.h>
#include <memory>

namespace common = viper::common;

auto main() -> int
{
    common::ArgumentParser parser {};
    auto& lex_parser = parser.addSubparser("lex");
    lex_parser.registerArg(
        std::make_shared<common::PositionalArgument>(
            "file"
            , "Name of the file to lex"
            , [](const common::PositionalArgument& arg) {
                std::cout << arg.getValue();
            }
        )
    );

    return 0;
}
