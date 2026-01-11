#ifndef VIPER_TOOLCHAIN_LEX_TOKEN_KIND_H
#define VIPER_TOOLCHAIN_LEX_TOKEN_KIND_H
#include <iostream>

namespace viper::toolchain::lex
{
    // This is used to define the kind of tokens that are available in the Viper language
    template <typename T>
    class [[nodiscard]] TokenKindBase
    {
        public:
            constexpr auto is_symbol() const noexcept -> bool { return T::IsSymbol; }

            constexpr auto is_grouping() const noexcept -> bool { return T::IsGrouping; }

            constexpr auto is_opening() const noexcept -> bool { return T::IsOpening; }

            constexpr auto is_keyword() const noexcept -> bool { return T::IsKeyword; }

            constexpr auto text_pattern() const noexcept -> const char* { return T::TextPattern; }

        private:
    };

// Create an enum for the 
#define VIPER_TOKEN_KIND(Name) Name,
#define VIPER_TOKEN_KIND_SYMBOL(Name, Pattern) Name,
#define VIPER_TOKEN_KIND_KEYWORD(Name, Pattern) Name,
    enum class TokenKind
    {
#include "tokens.def"
    };
#undef VIPER_TOKEN_KIND
#undef VIPER_TOKEN_KIND_KEYWORD
#undef VIPER_TOKEN_KIND_SYMBOL 

// Get the string form of each TokenKind name
#define VIPER_TOKEN_KIND(Name) case TokenKind::Name: return #Name;
#define VIPER_TOKEN_KIND_SYMBOL(Name, Pattern) case TokenKind::Name: return #Name;
#define VIPER_TOKEN_KIND_KEYWORD(Name, Pattern) case TokenKind::Name: return #Name;
    inline auto getTokenKindString(TokenKind kind) -> const char*
    {
        switch (kind)
        {
#include "tokens.def"
        }
    }
#undef VIPER_TOKEN_KIND
#undef VIPER_TOKEN_KIND_KEYWORD
#undef VIPER_TOKEN_KIND_SYMBOL

#define VIPER_TOKEN_KIND_SYMBOL(Name, Pattern) if (Pattern[0] == c) return true; 
    inline auto symbolStartsWith(char c) noexcept -> bool
    {
#include "tokens.def"
        return false;
    }
#undef VIPER_TOKEN_KIND_SYMBOL
#undef VIPER_TOKEN_KIND_KEYWORD
#undef VIPER_TOKEN_KIND

} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKEN_KIND_H
