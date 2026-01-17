#include "tests/lex_tests.h"
#include "base/real.h"
#include "base/shared_values.h"
#include "common/format.h"
#include "common/streams.h"
#include "diagnostics/consumer.h"
#include "lex.h"
#include "lexer.h"
#include "token_kind.h"
#include <set>
#include <string>
#include <vector>

namespace viper::toolchain::lex
{
    auto lexKeywordTest() -> std::optional<std::string>
    {
        base::SharedValues mock_shared_values {};
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
        
        auto tokens = lex(source.value(), mock_consumer, mock_shared_values).tokens();

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
        base::SharedValues mock_shared_values {};
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
        
        auto tokens = lex(source.value(), mock_consumer, mock_shared_values).tokens();

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
    
    auto lexIdentifiers() -> std::optional<std::string>
    {
        base::SharedValues mock_shared_values {};
        const std::string path { "tests/lex/identifiers.viper" };
        const std::vector<lex::TokenKind> expected = {
            TokenKind::Define,
            TokenKind::Id,
            TokenKind::LeftParen,
            TokenKind::RightParen,
            TokenKind::MinusGreater,
            TokenKind::Id,
            TokenKind::LeftCurly,
            TokenKind::Let,
            TokenKind::Id,
            TokenKind::Colon,
            TokenKind::Id,
            TokenKind::Equal,
            TokenKind::IntLiteral,
            TokenKind::SemiColon,
            TokenKind::Return,
            TokenKind::Id,
            TokenKind::SemiColon,
            TokenKind::RightCurly,
        };
        const std::set<std::string_view> expected_ident_names = {
            "main",
            "i32",
            "i",
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
        
        auto tokens = lex(source.value(), mock_consumer, mock_shared_values).tokens();

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

        if (mock_shared_values.identifiers().size() != expected_ident_names.size())
        {
            return format::format("Read {} identifiers when expected {}", mock_shared_values.identifiers().size(), expected_ident_names.size());
        }

        index = 0;
        for (const auto& expected : expected_ident_names)
        {
            bool found { false };
            for (const auto& id : mock_shared_values.identifiers())
            {
                if (id == expected)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                return format::format("Failed to find expected identifier [{}]", expected);
            }
        }
        
        for (const auto& id : mock_shared_values.identifiers())
        {
            bool found { false };
            for (const auto& expected : expected_ident_names)
            {
                if (id == expected)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                return format::format("Found unexpected identifier [{}]", id);
            }
        }

        return {};
    }
   
    auto lexNumbers() -> std::optional<std::string>
    {
        base::SharedValues mock_shared_values {};
        const std::string path { "tests/lex/numbers.viper" };
        const std::vector<lex::TokenKind> expected = {
            TokenKind::IntLiteral,
            TokenKind::RealLiteral,
            TokenKind::IntLiteral,
            TokenKind::RealLiteral,
            TokenKind::IntLiteral,
            TokenKind::RealLiteral,
            TokenKind::IntLiteral,
        };
        const std::vector<int64_t> expected_integers = {
            1,
            0,
            100,
            1234
        };
        const std::vector<base::Real> expected_reals = {
            { .mantissa = 1, .exponent = 0 },
            { .mantissa = 0, .exponent = 0 },
            { .mantissa = 1, .exponent = 234 },
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
        
        auto tokens = lex(source.value(), mock_consumer, mock_shared_values).tokens();

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

        if (mock_shared_values.integers().size() != expected_integers.size())
        {
            return format::format("Read {} integers when expected {}", mock_shared_values.integers().size(), expected_integers.size());
        }

        index = 0;
        for (const auto& expected : expected_integers)
        {
            bool found { false };
            for (const auto& integer : mock_shared_values.integers())
            {
                if (integer == expected)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                return format::format("Failed to find expected integer [{}]", expected);
            }
        }
        
        for (const auto& integer : mock_shared_values.integers())
        {
            bool found { false };
            for (const auto& expected : expected_integers)
            {
                if (integer == expected)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                return format::format("Found unexpected identifier [{}]", integer);
            }
        }

        index = 0;
        for (const auto& expected : expected_reals)
        {
            bool found { false };
            for (const auto& real : mock_shared_values.reals())
            {
                if (real.mantissa == expected.mantissa && real.exponent == expected.exponent)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                return format::format("Failed to find expected real [{} : {}]", expected.mantissa, expected.exponent);
            }
        }
        
        for (const auto& real : mock_shared_values.reals())
        {
            bool found { false };
            for (const auto& expected : expected_reals)
            {
                if (real.mantissa == expected.mantissa && real.exponent == expected.exponent)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                return format::format("Found unexpected real [{} : {}]", real.mantissa, real.exponent);
            }
        }

        return {};
    }
} // namespace viper::toolchain::lex
