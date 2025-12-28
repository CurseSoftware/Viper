#include "lex.h"
#include "token_kinds.h"
#include <iostream>

namespace viper::toolchain::lex
{
    auto lex() -> void
    {
        auto token = LetTokenKind();
        std::cout << "Is symbol: " << token.is_symbol() << '\n';
        std::cout << "Text pattern: " << token.text_pattern() << '\n';
        std::cout << "Is Grouping: " << token.is_grouping() << '\n';
        std::cout << "Is Keyword: " << token.is_keyword() << '\n';
    }
} // namespace viper::toolchain::lex
