#include "test_subcommand.h"
#include "common/streams.h"
#include "compilation_unit.h"
#include "diagnostics/consumer.h"
#include "lex/lex.h"
#include "lex/lexer.h"
#include "lex/token_kind.h"
#include <cstddef>
#include <filesystem>
#include <ostream>
#include <string>
namespace viper::toolchain::driver
{
    auto TestCommand::build(cli::Command& command) noexcept -> void
    {
        _output_arg = command.addOptional<std::string>("output", "Output stream for testing. Defaults to std::cout if not specified", NoOutputStreamSpecified);
    }

    auto TestCommand::run() noexcept -> void
    {
        // TODO: support sending this to file

        createLexTests();

        _manager.runAll();
    }

    auto lexKeywordTest() -> bool
    {
        const std::string path { "tests/lex/keywords.viper" };
        const std::vector<lex::TokenKind> expected = {
            lex::TokenKind::Define,
            lex::TokenKind::Let,
            lex::TokenKind::Mut,
        };
        
        auto null_out = NullOStream();
        auto mock_consumer = std::make_shared<diagnostics::StreamConsumer>(null_out);
        auto unit = CompilationUnit(path, mock_consumer);
        
        unit.tokenize();
        auto tokens = unit.tokens().tokens();

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
    
    auto TestCommand::createLexTests() noexcept -> void
    {
        _manager.registerTest("Lex Keywords", lexKeywordTest);
    }

} // namespace viper::toolchain::driver
