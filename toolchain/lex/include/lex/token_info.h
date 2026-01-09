#ifndef VIPER_TOOLCHAIN_LEX_TOKEN_INFO_H
#define VIPER_TOOLCHAIN_LEX_TOKEN_INFO_H
#include "token_kinds.h"

namespace viper::toolchain::lex
{
    class [[nodiscard]] TokenInfo
    {
        public:
            explicit TokenInfo(TokenKind kind) : _kind{ kind } {}
            
            auto kind() const noexcept -> TokenKind { return _kind; }
            
        private:
            TokenKind _kind;
    };

} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TOKEN_INFO_H
