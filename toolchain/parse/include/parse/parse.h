#ifndef VIPER_TOOLCHAIN_PARSE_PARSE_H
#define VIPER_TOOLCHAIN_PARSE_PARSE_H

#include "diagnostics/consumer.h"
#include "lex/tokenized_buffer.h"
#include "tree.h"

#include <memory>

namespace viper::toolchain::parse
{
    // Takes an input tokenzized buffer and generates a parse::Tree from it
    auto parse(lex::TokenizedBuffer& tokens, std::weak_ptr<diagnostics::Consumer> diagnostics_consumer) -> Tree;
} // namespace viper::toolchain::parse

#endif // VIPER_TOOLCHAIN_PARSE_PARSE_H
