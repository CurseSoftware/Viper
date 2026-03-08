#include "context.h"
#include "lex/token_kind.h"

namespace viper::toolchain::parse
{
    auto ParseContext::parse() noexcept -> Tree
    {
        return std::move(_tree);
    }

    auto ParseContext::parseFile() noexcept -> void
    {
        
    }

    auto ParseContext::currentPositionKind() const noexcept -> lex::TokenKind
    {
        return _tokens.tokens().get(*_position)
            .value()
            .get()
            .kind();
    }

    auto ParseContext::currentPositionIs(lex::TokenKind kind) const noexcept -> bool
    {
        return currentPositionKind() == kind;
    }

    auto ParseContext::consume() noexcept -> lex::TokenIndex
    {
        return *(_position++);
    }

    auto ParseContext::consumeChecked(lex::TokenKind kind) noexcept -> lex::TokenIndex
    {
        if (!currentPositionIs(kind))
        {
            std::cerr << "Fatal: consumeChecked failed\n";
        }

        return *(_position++);
    }
} // namespace viper::toolchain::parse
