#ifndef VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H
#define VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H

#include "common/containers/store.h"
#include "lex/token_info.h"
#include <cstddef>
#include <vector>
namespace viper::toolchain::lex
{
    struct TokenIndex : public containers::Index<TokenIndex>
    {
        using Index::Index;
        static const TokenIndex None;
    };
    
    class [[nodiscard]] TokenizedBuffer
    {
        public:

        public:
            auto dumpTokens() const noexcept -> void;

            auto size() const noexcept -> std::size_t { return _token_infos.size(); }

            auto tokens() const noexcept -> const containers::ValueStore<TokenIndex, TokenInfo>& { return _token_infos; }

            // Add a new token into this buffer
            auto addToken(TokenInfo info) noexcept -> TokenIndex;

            auto has_errors() const noexcept -> bool { return _has_errors; }

            auto setHasErrors(bool has_errors) noexcept -> void { _has_errors = has_errors; }

        private:
            containers::ValueStore<TokenIndex, TokenInfo> _token_infos;
            // std::vector<TokenInfo> _token_infos;
            bool _has_errors { false };
    };
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H
