#include "tokenized_buffer.h"
#include "token_kind.h"
#include <iostream>

namespace viper::toolchain::lex
{
    auto TokenizedBuffer::addToken(TokenInfo info) noexcept -> TokenIndex
    {
        // _token_infos.push_back(info);
        auto id = _token_infos.add(info);
        return id;
        
        // return _token_infos.size()-1;
    }

    auto TokenizedBuffer::dumpTokens() const noexcept -> void
    {
        for (const auto info : _token_infos)
        {
            std::cout << "Token: " << getTokenKindString(info.kind()) << '\n';
        }
    }
} // namespace viper::toolchain::lex
