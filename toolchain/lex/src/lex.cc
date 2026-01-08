#include "lex.h"
#include "source/source_buffer.h"
#include "token_kinds.h"
#include "tokenized_buffer.h"
#include <iostream>

namespace viper::toolchain::lex
{
    auto lex(source::SourceBuffer& source_buffer) -> TokenizedBuffer
    {
        (void)source_buffer;
        auto token = LetTokenKind();
        std::cout << "Is symbol: " << token.is_symbol() << '\n';
        std::cout << "Text pattern: " << token.text_pattern() << '\n';
        std::cout << "Is Grouping: " << token.is_grouping() << '\n';
        std::cout << "Is Keyword: " << token.is_keyword() << '\n';

        return {};
    }
} // namespace viper::toolchain::lex
