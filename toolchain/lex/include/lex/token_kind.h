#ifndef VIPER_TOOLCHAIN_LEX_TOKEN_KIND_H
#define VIPER_TOOLCHAIN_LEX_TOKEN_KIND_H

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
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKEN_KIND_H
