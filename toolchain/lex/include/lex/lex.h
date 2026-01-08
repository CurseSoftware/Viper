#ifndef VIPER_TOOLCHAIN_LEX_LEX_H
#define VIPER_TOOLCHAIN_LEX_LEX_H
#include "source/source_buffer.h"
#include "token_kind.h"
#include "tokenized_buffer.h"

namespace viper::toolchain::lex
{
    // Lex the input source buffer
    auto lex(source::SourceBuffer& source_buffer) -> TokenizedBuffer;
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_LEX_H
