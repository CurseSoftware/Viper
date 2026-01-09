#include "lex.h"
#include "characters.h"
#include "diagnostics/consumer.h"
#include "diagnostics/diagnostic.h"
#include "diagnostics/kinds.h"
#include "lexer.h"
#include "source/source_buffer.h"
#include "token_kind.h"
#include "token_kinds.h"
#include "tokenized_buffer.h"
#include "tokens.h"
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string_view>

namespace viper::toolchain::lex
{
    // The size of the table for dispatching lexer calls depending on character
    constexpr int DispatchTableSize = 256;
    using LexDispatchFunctionT = auto(Lexer& lexer, std::string_view text, std::size_t position) -> void;
    using DispatchTableT = std::array<LexDispatchFunctionT*, DispatchTableSize>;

    // The size of the tables for determining valid characters for identifiers
    constexpr int IdByteTableSize = 256;

    static constexpr TokenSpec Spec = TokenSpec::specify()
        .addKeyword(TokenSpecInfo("let", TokenKind::Let))
        .addKeyword(TokenSpecInfo("return", TokenKind::Return))
        .addKeyword(TokenSpecInfo("define", TokenKind::Define))
        .addKeyword(TokenSpecInfo("mut", TokenKind::Mut))
        .identifierCanStartWith('-')
        .identifierCanStartWithLower()
        .identifierCanStartWithUpper()
        .identifierCanInclude('-')
        .identifierCanIncludeLower()
        .identifierCanIncludeUpper()
        .identifierCanIncludeNumeric()
    ;

    // This is the table of characters that is valid for the start of an identifier
    static constexpr std::array<bool, IdByteTableSize> IdStartBtyteTable = [] {
        std::array<bool, IdByteTableSize> table = {};

        for (char c = 'A'; c <= 'Z'; c++)
        {
            table[c] = true;
        }
        for (char c = 'a'; c <= 'z'; c++)
        {
            table[c] = true;
        }

        table['_'] = true;

        return table;
    }();

    // This is the byte table for all characters that are valid after the start of an identifier
    static constexpr std::array<bool, IdByteTableSize> IdByteTable = [] {
        std::array<bool, IdByteTableSize> table = IdStartBtyteTable;

        for (char c = '0'; c <= '9'; c++)
        {
            table[c] = true;
        }

        return table;
    }();

    // Scan for the text of an identifier
    auto Lexer::scanIdentifier(std::string_view text, std::size_t i) -> std::string_view
    {
        const std::size_t size = text.size();
        while (i < size && _token_spec.isValidIdChar(text[i]))
        {
            i++;
        }

        return text.substr(0, i);
    }
    
    auto Lexer::skipVerticalWhitespace(std::string_view text, std::size_t i) -> std::size_t
    {
        std::size_t new_position = i;
        while (isVerticalWhitespace(text[new_position]))
        {
            new_position++;
        }

        return new_position;
    }

    auto Lexer::skipHorizontalWhitespace(std::string_view text, std::size_t i) -> std::size_t
    {
        std::size_t new_position = i;
        while (isHorizontalWhitespace(text[new_position]))
        {
            new_position++;
        }

        return new_position;
    }

    auto Lexer::lexHorizontalWhitespace(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        position = skipHorizontalWhitespace(text, position);
        return Result();
    }

    auto Lexer::lexVerticalWhitespace(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        position = skipVerticalWhitespace(text, position);
        return Result();
    }

    auto Lexer::lexError(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        std::size_t i = position;
        do {
            std::cout << "Invaid char: " << text[i] << '\n';
            i++;
        }
        while (!_token_spec.isValidIdChar(text[i]) 
            && !isNumeric(text[i])
            && !isHorizontalWhitespace(text[i])
            && !isVerticalWhitespace(text[i])
            && !symbolStartsWith(text[i])
        );

        std::string_view error_text = text.substr(position, i-position);

        diagnostics::make_diagnostic<diagnostics::InvalidCharactersDiagnostic>(diagnostics::Level::Error, std::string(error_text));

        position += error_text.size();
        // std::cout << "Error: " << error_text << '\n';

        addLexedToken(TokenKind::Error);

        return Lexer::Result::invalid();
    }

    auto Lexer::lexKeywordOrIdentifier(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        if (static_cast<unsigned char>(text[position]) > 0x7F)
        {
            // TODO: return lex error for not supporting utf8
            return Result::invalid();
        }

        int32_t byte_offset = position;

        std::string_view id_text = scanIdentifier(text.substr(position), 0);
        std::cout << "Lexed id: " << id_text << " at position " << position << '\n';

        position += id_text.length();

        if (auto keyword_kind = _token_spec.keywordKind(id_text))
        {
            addLexedToken(keyword_kind.value());
            return Result();
        }

        addLexedToken(TokenKind::Id);

        return Result();
    }

    static auto dispatchNext(Lexer& lexer, std::string_view text, std::size_t position) -> void;


#define VIPER_DISPATCH_LEX_TOKEN(LexMethod) \
    static auto Dispatch_##LexMethod(Lexer& lexer, std::string_view text, std::size_t position) -> void \
    { \
        Lexer::Result result = lexer.LexMethod(text, position); \
        if (!result) \
        { \
        } \
        return dispatchNext(lexer, text, position); \
    }

    VIPER_DISPATCH_LEX_TOKEN(lexError)
    VIPER_DISPATCH_LEX_TOKEN(lexKeywordOrIdentifier)
    VIPER_DISPATCH_LEX_TOKEN(lexHorizontalWhitespace)
    VIPER_DISPATCH_LEX_TOKEN(lexVerticalWhitespace)


    // Create the dispatch table
    static consteval auto makeDispatchTable() -> DispatchTableT
    {
        DispatchTableT table {};

        for (int i = 0; i < DispatchTableSize; i++)
        {
            table[i] = Dispatch_lexError;
        }

        for (char c = 'a'; c <= 'z'; c++)
        {
            table[c] = Dispatch_lexKeywordOrIdentifier;
        }

        for (char c = 'A'; c <= 'z'; c++)
        {
            table[c] = Dispatch_lexKeywordOrIdentifier;
        }

        table[' '] = Dispatch_lexHorizontalWhitespace;
        table['\t'] = Dispatch_lexHorizontalWhitespace;
        table['\n'] = Dispatch_lexVerticalWhitespace;
        table['\r'] = Dispatch_lexVerticalWhitespace;

        return table;
    }

    static constexpr DispatchTableT DispatchTable = makeDispatchTable();

    static auto dispatchNext(Lexer& lexer, std::string_view text, std::size_t position) -> void
    {

        // std::cout << "Lexer encountered char: " << text[position]  << " at position " << position << '\n';
        [[likely]] if (position < text.size())
        {
            // This is tail recursion to avoid growing stack while doing this loop. 
            // See https://github.com/carbon-language/carbon-lang/blob/trunk/toolchain/lex/lex.cpp
            // NOLINTNEXTLINE(readability-avoid-return-with-void-value): For musttail.
            [[clang::musttail]] return DispatchTable[static_cast<unsigned char>(text[position])](lexer, text, position);
        }
    }

    auto Lexer::lex() && noexcept -> TokenizedBuffer
    {
        std::string_view source_text = _source.getBuffer();

        std::size_t position { 0 };
        dispatchNext(*this, source_text, position);

        return std::move(_buffer);
    }

    auto Lexer::addLexedToken(TokenKind kind) noexcept -> void
    {
        _buffer.addToken(TokenInfo(kind));
    }

    auto lex(source::SourceBuffer& source_buffer, std::weak_ptr<diagnostics::Consumer> consumer) -> TokenizedBuffer
    {
        Spec.printKeywords();
        auto tokens = Lexer(source_buffer, consumer, Spec).lex();
        tokens.dumpTokens();

        return std::move(tokens);
    }
} // namespace viper::toolchain::lex
