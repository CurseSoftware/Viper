#ifndef VIPER_TOOLCHAIN_LEX_CHARACTERS_H
#define VIPER_TOOLCHAIN_LEX_CHARACTERS_H

#include <string_view>
namespace viper::toolchain::lex
{
    inline auto isLower(char c) -> bool { return 'a' <= c && c <= 'z'; }

    inline auto isUpper(char c) -> bool { return 'A' <= c && c <= 'Z'; }

    inline auto isAlpha(char c) -> bool { return isLower(c) || isUpper(c); }

    inline auto isNumeric(char c) -> bool { return '0' <= c && c <= '9'; }

    inline auto isAlphaNumeric(char c) -> bool { return isAlpha(c) || isNumeric(c); }

    inline auto isAscii(char c) -> bool
    {
        return static_cast<unsigned char>(c) <= 127;
    }

    inline auto isEof(char c) -> bool { return c == '\0'; }

    inline auto isAscii(std::string_view s)
    {
        for (const auto c : s)
        {
            [[unlikely]] if (!isAscii(c))
            {
                return false;
            }
        }

        return true;
    }

    inline auto isHorizontalWhitespace(char c) -> bool
    {
        return c == ' ' || c == '\t';
    }

    inline auto isVerticalWhitespace(char c) -> bool
    {
        return c == '\r' || c == '\n';
    }
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_CHARACTERS_H
