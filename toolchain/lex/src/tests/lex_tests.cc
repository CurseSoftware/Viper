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

        for (std::size_t i = 0; i < tokens.size(); i++)
        {
            if (expected[i] != tokens[i].kind())
            {
                return format::format("tokens[i] = {} when expected {}", getTokenKindString(tokens[i].kind()), getTokenKindString(expected[i]));
            }
        }

        return {};
    }
} // namespace viper::toolchain::lex
