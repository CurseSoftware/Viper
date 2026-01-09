#ifndef VIPER_TOOLCHAIN_LEX_TOKENS_H
#define VIPER_TOOLCHAIN_LEX_TOKENS_H

#include "token_info.h"
#include "token_kind.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <optional>
#include <unordered_map>
namespace viper::toolchain::lex
{
    class [[nodiscard]] TokenSpecInfo
    {
        public:
            consteval explicit TokenSpecInfo(const char* pattern, TokenKind kind)
                : _pattern{ pattern }
                , _kind{ kind }
            {}

            constexpr auto pattern() const noexcept -> const char* { return _pattern; }
            constexpr auto kind() const noexcept -> TokenKind { return _kind; }

        private:
            const char* _pattern;
            TokenKind _kind;
            std::optional<TokenKind> _closing { std::nullopt };
            std::optional<TokenKind> _opening { std::nullopt };
    };
    
    class TokenSpec
    {
        public:
            consteval static auto specify() -> TokenSpec
            {
                return TokenSpec();
            }

            enum Kind
            {
                IdStart,
                Id,
                Symbol
            };

        public:
            consteval auto addKeyword(TokenSpecInfo info) -> TokenSpec
            {
                _keywords[_keyword_index] = info;
                _keyword_index++;
                return *this;
            }

            consteval auto addSymbol(TokenSpecInfo info) -> TokenSpec
            {
                _symbol_start_byte_table[info.pattern()[0]] = true;
                _symbols[_symbol_index] = info;
                _symbol_index++;
                return *this;
            }
            
            consteval auto identifierCanStartWith(char c) -> TokenSpec
            {
                _id_start_byte_table[c] = true;
                return *this;
            }

            consteval auto identifierCanStartWithNumeric() -> TokenSpec
            {
                for (char c = '0'; c <= '9'; c++)
                {
                    _id_start_byte_table[c] = true;
                }

                return *this;
            }

            consteval auto identifierCanStartWithLower() -> TokenSpec
            {
                for (char c = 'a'; c <= 'z'; c++)
                {
                    _id_start_byte_table[c] = true;
                }

                return *this;
            }

            consteval auto identifierCanStartWithUpper() -> TokenSpec
            {
                for (char c = 'A'; c <= 'Z'; c++)
                {
                    _id_start_byte_table[c] = true;
                }

                return *this;
            }

            consteval auto identifierCanInclude(char c) -> TokenSpec
            {
                _id_byte_table[c] = true;
                return *this;
            }

            consteval auto identifierCanIncludeNumeric() -> TokenSpec
            {
                for (char c = '0'; c <= '9'; c++)
                {
                    _id_byte_table[c] = true;
                }

                return *this;
            }

            consteval auto identifierCanIncludeLower() -> TokenSpec
            {
                for (char c = 'a'; c <= 'z'; c++)
                {
                    _id_byte_table[c] = true;
                }

                return *this;
            }

            consteval auto identifierCanIncludeUpper() -> TokenSpec
            {
                for (char c = 'A'; c <= 'Z'; c++)
                {
                    _id_byte_table[c] = true;
                }

                return *this;
            }

        // Non constant expression API
        public:
            auto printKeywords() const
            {
                for (const auto& keyword : _keywords)
                {
                    if (keyword)
                    {
                        std::cout << "Keyword: " << keyword.value().pattern() << '\n';
                    }
                }
            }

            auto printIdTable() const
            {
                for (std::size_t i = 0; i < _id_byte_table.size(); i++)
                {
                    std::cout << i << ": " <<_id_byte_table[i] << " ";
                }
            }

            auto printIdStartTable() const
            {
                for (std::size_t i = 0; i < _id_start_byte_table.size(); i++)
                {
                    std::cout << i << ": " << _id_start_byte_table[i] << " ";
                }
            }
            

            [[nodiscard]] auto isValidIdChar(char c) const -> bool
            {
                return _id_byte_table[c];
            }

            [[nodiscard]] auto isValidIdStartChar(char c) const -> bool
            {
                return _id_start_byte_table[c];
            }

            [[nodiscard]] auto isKeyword(std::string_view s) const -> bool
            {
                for (const auto& keyword : _keywords)
                {
                    if (keyword && s == keyword->pattern())
                    {
                        return true;
                    }
                }

                return false;
            }

            [[nodiscard]] auto keywordKind(std::string_view s) const -> std::optional<TokenKind>
            {
                for (const auto& keyword : _keywords)
                {
                    if (keyword && s == keyword->pattern())
                    {
                        return keyword->kind();
                    }
                }

                return std::nullopt;
            }

        private:
            int _keyword_index = 0;
            std::array<std::optional<TokenSpecInfo>, 20> _keywords;
            
            int _symbol_index = 0;
            std::array<std::optional<TokenSpecInfo>, 20> _symbols;

            std::array<bool, 256> _id_start_byte_table {};
            std::array<bool, 256> _id_byte_table {};
            
            std::array<bool, 256> _symbol_start_byte_table {};
    };
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKENS_H
