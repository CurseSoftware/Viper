#ifndef VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H
#define VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H

#include "lex/token_info.h"
#include <cstddef>
#include <vector>
namespace viper::toolchain::lex
{
    using TokenIndex = std::size_t;
    
    class TokenizedBuffer
    {
        public:

        public:
            auto dumpTokens() const noexcept -> void;

            auto size() const noexcept -> std::size_t { return _token_infos.size(); }

            auto tokens() const noexcept -> const std::vector<TokenInfo>& { return _token_infos; }

            // Add a new token into this buffer
            auto addToken(TokenInfo info) noexcept -> TokenIndex;

        private:
            std::vector<TokenInfo> _token_infos;
    };
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H
