#include "lex.h"
#include "characters.h"
#include "diagnostics/consumer.h"
#include "lexer.h"
#include "source/source_buffer.h"
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
        .addKeyword("let")
        .addKeyword("define")
        .addKeyword("mut")
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

        std::cout << "Read " << i << " characters read: " << text.substr(0, i) << '\n';

        return text.substr(0, i);
    }
    
    auto Lexer::skipVerticalWhitespace(std::string_view text, std::size_t i) -> std::size_t
    {
        std::cout << "Skipping vertical whitespace\n";
        std::size_t new_position = i;
        while (isVerticalWhitespace(text[new_position]))
        {
            new_position++;
        }

        return new_position;
    }

    auto Lexer::skipHorizontalWhitespace(std::string_view text, std::size_t i) -> std::size_t
    {
        std::cout << "Skipping horizontal whitespace\n";
        std::size_t new_position = i;
        while (isHorizontalWhitespace(text[new_position]))
        {
            new_position++;
        }

        return new_position;
    }

    auto Lexer::lexHorizontalWhitespace(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        position += skipHorizontalWhitespace(text, position);
        return Result();
    }

    auto Lexer::lexVerticalWhitespace(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        position += skipVerticalWhitespace(text, position);
        return Result();
    }

    auto Lexer::lexKeywordOrIdentifier(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        std::cout << "lexing keyword\n";
        if (static_cast<unsigned char>(text[position]) > 0x7F)
        {
            // TODO: return lex error for not supporting utf8
            std::cout << "cannot lex utf-8\n";
            return Result::invalid();
        }

        int32_t byte_offset = position;

        std::cout << "Substring: " << text.substr(position) << '\n';
        std::string_view id_text = scanIdentifier(text.substr(position), 0);

        position += id_text.length();

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

    VIPER_DISPATCH_LEX_TOKEN(lexKeywordOrIdentifier)
    VIPER_DISPATCH_LEX_TOKEN(lexHorizontalWhitespace)
    VIPER_DISPATCH_LEX_TOKEN(lexVerticalWhitespace)


    // Create the dispatch table
    static consteval auto makeDispatchTable() -> DispatchTableT
    {
        DispatchTableT table {};

        for (int i = 0; i < DispatchTableSize; i++)
        {
            // set to dispatch an error
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
    std::cout << "Read char " << position << ": " << text[position] << '\n';
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

        return {};
    }

    auto lex(source::SourceBuffer& source_buffer, std::weak_ptr<diagnostics::Consumer> consumer) -> TokenizedBuffer
    {
        (void)source_buffer;
        auto token = LetTokenKind();
        std::cout << "Is symbol: " << token.is_symbol() << '\n';
        std::cout << "Text pattern: " << token.text_pattern() << '\n';
        std::cout << "Is Grouping: " << token.is_grouping() << '\n';
        std::cout << "Is Keyword: " << token.is_keyword() << '\n';

        Spec.printKeywords();
        auto tokens = Lexer(source_buffer, consumer, Spec).lex();

        return {};
    }
} // namespace viper::toolchain::lex
