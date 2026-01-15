#ifndef VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H
#define VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H

#include "common/containers/store.h"
#include "lex/token_info.h"
#include <cstddef>

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
            // Print all the tokens
            auto dumpTokens() const noexcept -> void;

            // Get the number of tokens stored
            auto size() const noexcept -> std::size_t { return _token_infos.size(); }

            // Getter for the actual token storage
            auto tokens() const noexcept -> const containers::Store<TokenIndex, TokenInfo>& { return _token_infos; }

            // Add a new token into this buffer
            auto addToken(TokenInfo info) noexcept -> TokenIndex;

            template <typename... Args>
            auto emplaceToken(Args&& ...args) noexcept -> TokenIndex
            {
                return _token_infos.emplace(args...);
            }

            // Pre-allocate space for the buffer
            auto reserve(int32_t size) noexcept -> void;

            // See whether this buffer contains errors
            auto has_errors() const noexcept -> bool { return _has_errors; }

            // Set whether this has errors or not
            auto setHasErrors(bool has_errors) noexcept -> void { _has_errors = has_errors; }

        private:
            containers::Store<TokenIndex, TokenInfo> _token_infos;
            // std::vector<TokenInfo> _token_infos;
            bool _has_errors { false };
    };
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKENIZED_BUFFER_H
