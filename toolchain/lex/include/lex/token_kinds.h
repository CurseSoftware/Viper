#ifndef VIPER_TOOLCHAIN_LEX_TOKEN_KINDS_H
#define VIPER_TOOLCHAIN_LEX_TOKEN_KINDS_H
#include "token_kind.h"

namespace viper::toolchain::lex
{

// ---------------------------------------------------------
// Symbol Tokens
// ---------------------------------------------------------
#define VIPER_TOKEN_KIND_SYMBOL(name, pattern) \
    class name##TokenKind : public TokenKindBase<name##TokenKind> \
    { \
        public: \
            static constexpr bool IsSymbol { true }; \
            static constexpr bool IsKeyword { false }; \
            static constexpr bool IsGrouping { false }; \
            static constexpr bool IsOpening { false }; \
            static constexpr bool IsClosing { false }; \
            static constexpr const char* TextPattern { pattern }; \
    };

// ---------------------------------------------------------
// Keyword Tokens
// ---------------------------------------------------------
#define VIPER_TOKEN_KIND_KEYWORD(name, pattern) \
    class name##TokenKind : public TokenKindBase<name##TokenKind> \
    { \
        public: \
            static constexpr bool IsSymbol { false }; \
            static constexpr bool IsKeyword { true }; \
            static constexpr bool IsGrouping { false }; \
            static constexpr bool IsOpening { false }; \
            static constexpr bool IsClosing { false }; \
            static constexpr const char* TextPattern { pattern }; \
    };

#include "tokens.def"

} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKEN_KINDS_H
