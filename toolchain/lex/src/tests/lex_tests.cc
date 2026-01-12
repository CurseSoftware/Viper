#include "tests/lex_tests.h"
#include "common/streams.h"
#include "diagnostics/consumer.h"
#include "lex.h"
#include "lexer.h"
#include "token_kind.h"
#include <string>
#include <vector>

namespace viper::toolchain::lex
{
    auto lexKeywordTest() -> bool
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
            return false;
        }
        
        auto tokens = lex(source.value(), mock_consumer).tokens();

        if (tokens.size() != expected.size())
        {
            return false;
        }

        for (std::size_t i = 0; i < tokens.size(); i++)
        {
            if (expected[i] != tokens[i].kind())
            {
                return false;
            }
        }

        return true;
    }
} // namespace viper::toolchain::lex
