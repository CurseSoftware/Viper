#ifndef VIPER_TOOLCHAIN_LEX_TOKENS_H
#define VIPER_TOOLCHAIN_LEX_TOKENS_H

#include "token_kind.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <unordered_map>
namespace viper::toolchain::lex
{
    class TokenSpec
    {
        public:
            consteval static auto specify() -> TokenSpec
            {
                return TokenSpec();
            }

        public:
            consteval auto addKeyword(const char* name) -> TokenSpec
            {
                _keywords[_keyword_index] = name;
                _keyword_index++;
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
                for (const auto* keyword : _keywords)
                {
                    if (keyword)
                    {
                        std::cout << "Keyword: " << keyword << '\n';
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

            [[nodiscard]] auto isKeyword(std::string_view s) -> bool
            {
                for (const auto* keyword : _keywords)
                {
                    if (keyword && s == keyword)
                    {
                        return true;
                    }
                }

                return false;
            }

        private:
            int _keyword_index = 0;
            std::array<const char*, 20> _keywords;

            std::array<bool, 256> _id_start_byte_table;
            std::array<bool, 256> _id_byte_table {};
    };
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKENS_H
