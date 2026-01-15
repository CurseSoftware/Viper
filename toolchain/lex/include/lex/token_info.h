#ifndef VIPER_TOOLCHAIN_LEX_TOKEN_INFO_H
#define VIPER_TOOLCHAIN_LEX_TOKEN_INFO_H
#include "token_kinds.h"
#include <cstdint>

namespace viper::toolchain::lex
{
    class [[nodiscard]] TokenInfo
    {
        public:
            explicit TokenInfo(TokenKind kind, int32_t byte_offset) noexcept
                : _kind{ kind }
                , _byte_offset{ byte_offset } 
            {}
            
            explicit TokenInfo(TokenKind kind, int32_t byte_offset, int32_t payload) noexcept
                : _kind{ kind }
                , _byte_offset{ byte_offset } 
                , _payload{ payload }
            {}
            
            // Get the kind of token this is
            auto kind() const noexcept -> TokenKind { return _kind; }

            // Get the byte offset of this token
            auto byteOffset() const noexcept -> int32_t { return _byte_offset; }
            
        private:
            // The kind of token this is
            TokenKind _kind;

            // The byte offset in the source text this token begins at
            int32_t _byte_offset;

            // This is used for various things.
            // Typically this is a lookup into different
            // shared value stores
            int32_t _payload { -1 };
    };

} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKEN_INFO_H
