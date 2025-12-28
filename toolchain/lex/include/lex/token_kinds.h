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

VIPER_TOKEN_KIND_SYMBOL(Equal, "=")
VIPER_TOKEN_KIND_SYMBOL(Plus, "+")
VIPER_TOKEN_KIND_SYMBOL(Minus, "-")
VIPER_TOKEN_KIND_SYMBOL(Asterisk, "*")
VIPER_TOKEN_KIND_SYMBOL(ForwardSlash, "/")
VIPER_TOKEN_KIND_SYMBOL(Comma, ",")
VIPER_TOKEN_KIND_SYMBOL(Period, ".")
VIPER_TOKEN_KIND_SYMBOL(Colon, ":")
VIPER_TOKEN_KIND_SYMBOL(SemiColon, "");

VIPER_TOKEN_KIND_SYMBOL(ColonColon, "::")
VIPER_TOKEN_KIND_SYMBOL(EqualEqual, "==")
VIPER_TOKEN_KIND_SYMBOL(PlusEqual, "+=")
VIPER_TOKEN_KIND_SYMBOL(MinusEqual, "-=")
VIPER_TOKEN_KIND_SYMBOL(AsteriskEqual, "*=")
VIPER_TOKEN_KIND_SYMBOL(ForwardSlashEqual, "/=")

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

VIPER_TOKEN_KIND_KEYWORD(Let, "let")
VIPER_TOKEN_KIND_KEYWORD(Define, "define")
VIPER_TOKEN_KIND_KEYWORD(Mut, "mut")

} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKEN_KINDS_H
