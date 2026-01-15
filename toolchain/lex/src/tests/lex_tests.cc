#include "tests/lex_tests.h"
#include "common/format.h"
#include "common/streams.h"
#include "diagnostics/consumer.h"
#include "lex.h"
#include "lexer.h"
#include "token_kind.h"
#include <string>
#include <vector>

namespace viper::toolchain::lex
{
    auto lexKeywordTest() -> std::optional<std::string>
    {
        const std::string path { "tests/lex/keywords.viper" };
        const std::vector<lex::TokenKind> expected = {
            TokenKind::Define,
            TokenKind::Let,
            TokenKind::Mut,
        };
        
        auto null_out = NullOStream();
        auto mock_consumer = std::make_shared<diagnostics::StreamConsumer>(null_out);
        // auto unit = driver::CompilationUnit(path, mock_consumer);
        
        // unit.tokenize();
        auto source = source::SourceBuffer::fromFilePath(path, mock_consumer);
        if (!source)
        {
            return format::format("Source buffer failed to create from file path \"{}\"", path);
        }
        
        auto tokens = lex(source.value(), mock_consumer).tokens();

        if (tokens.size() != expected.size())
        {
            return format::format("Read {} tokens when expected {}", tokens.size(), expected.size());
        }

        std::size_t index = 0;
        for (auto token : tokens)
        {
            if (expected[index] != token.kind())
            {
                return format::format("tokens[{}] = {} when expected {}", index, getTokenKindString(token.kind()), getTokenKindString(expected[index]));
            }
            index++;
        }

        return {};
    }
    
    auto lexSymbolTest() -> std::optional<std::string>
    {
        const std::string path { "tests/lex/symbols.viper" };
        const std::vector<lex::TokenKind> expected = {
            TokenKind::Plus,
            TokenKind::Plus,
            TokenKind::Equal,
            TokenKind::PlusEqual,
            TokenKind::EqualEqual,
            TokenKind::Equal,
            TokenKind::MinusEqual,
            TokenKind::ForwardSlash,
            TokenKind::ForwardSlashEqual,
            TokenKind::MinusGreater,
            TokenKind::SemiColon,
            TokenKind::ColonColon,
            TokenKind::Colon,
            TokenKind::ColonColon,
        };
        
        auto null_out = NullOStream();
        auto mock_consumer = std::make_shared<diagnostics::StreamConsumer>(null_out);
        // auto unit = driver::CompilationUnit(path, mock_consumer);
        
        // unit.tokenize();
        auto source = source::SourceBuffer::fromFilePath(path, mock_consumer);
        if (!source)
        {
            return format::format("Source buffer failed to create from file path \"{}\"", path);
        }
        
        auto tokens = lex(source.value(), mock_consumer).tokens();

        if (tokens.size() != expected.size())
        {
            return format::format("Read {} tokens when expected {}", tokens.size(), expected.size());
        }

        std::size_t index { 0 };
        for (auto token : tokens)
        {
            if (expected[index] != token.kind())
            {
                return format::format("tokens[{}] = {} when expected {}", index, getTokenKindString(token.kind()), getTokenKindString(expected[index]));
            }
            index++;
        }

        return {};
    }
} // namespace viper::toolchain::lex
