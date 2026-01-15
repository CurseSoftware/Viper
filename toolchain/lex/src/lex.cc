#include "lex.h"
#include "base/shared_values.h"
#include "characters.h"
#include "diagnostics/consumer.h"
#include "diagnostics/diagnostic.h"
#include "diagnostics/kinds.h"
#include "lexer.h"
#include "source/source_buffer.h"
#include "token_kind.h"
#include "tokenized_buffer.h"
#include "tokens.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <exception>
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
        .addKeyword(TokenSpecInfo("ref", TokenKind::Ref))
        .addKeyword(TokenSpecInfo("struct", TokenKind::Struct))
        .identifierCanStartWith('_')
        .identifierCanStartWithLower()
        .identifierCanStartWithUpper()
        .identifierCanInclude('_')
        .identifierCanIncludeLower()
        .identifierCanIncludeUpper()
        .identifierCanIncludeNumeric()
        .addSymbol(TokenSpecInfo("->", TokenKind::MinusGreater))
        .addSymbol(TokenSpecInfo("(", TokenKind::LeftParen))
        .addSymbol(TokenSpecInfo(")", TokenKind::RightParen))
        .addSymbol(TokenSpecInfo("==", TokenKind::EqualEqual))
        .addSymbol(TokenSpecInfo("+=", TokenKind::PlusEqual))
        .addSymbol(TokenSpecInfo("/=", TokenKind::ForwardSlashEqual))
        .addSymbol(TokenSpecInfo("-=", TokenKind::MinusEqual))
        .addSymbol(TokenSpecInfo("*=", TokenKind::AsteriskEqual))
        .addSymbol(TokenSpecInfo("+", TokenKind::Plus))
        .addSymbol(TokenSpecInfo("-", TokenKind::Minus))
        .addSymbol(TokenSpecInfo("=", TokenKind::Equal))
        .addSymbol(TokenSpecInfo("/", TokenKind::ForwardSlash))
        .addSymbol(TokenSpecInfo("::", TokenKind::ColonColon))
        .addSymbol(TokenSpecInfo(":", TokenKind::Colon))
        .addSymbol(TokenSpecInfo(";", TokenKind::SemiColon))
        .addSymbol(TokenSpecInfo("[", TokenKind::LeftBracket))
        .addSymbol(TokenSpecInfo("]", TokenKind::RightBracket))
        .addSymbol(TokenSpecInfo("{", TokenKind::LeftCurly))
        .addSymbol(TokenSpecInfo("}", TokenKind::RightCurly))
    ;

    // Scan for the text of an identifier
    auto Lexer::scanIdentifier(std::string_view text, std::size_t i) -> std::string_view
    {
        // std::cout << "Scan i: " << i << '\n';
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

    auto Lexer::lexHorizontalWhitespace(std::string_view text, std::size_t& position) noexcept -> void
    {
        position = skipHorizontalWhitespace(text, position);
    }

    auto Lexer::lexVerticalWhitespace(std::string_view text, std::size_t& position) noexcept -> void
    {
        position = skipVerticalWhitespace(text, position);
    }

    auto Lexer::lexNumericLiteral(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        int32_t byte_offset { static_cast<int32_t>(position) };
        if (!isNumeric(text[position]))
        {
            return Result::invalid();
        }

        std::size_t i { position };
        for (; isNumeric(text[i]) || text[i] == '.'; i++)
        {
        }
        // std::cout << "Numeric literal: " << text.substr(position, i-position) << '\n';
        position = i;
        return addLexedToken(TokenKind::NumericLiteral, byte_offset);
    }

    auto Lexer::lexError(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        int32_t byte_offset { static_cast<int32_t>(position) };
        std::size_t i = position;
        do {
            i++;
        }
        while (!_token_spec.isValidIdChar(text[i]) 
            && !isNumeric(text[i])
            && !isHorizontalWhitespace(text[i])
            && !isVerticalWhitespace(text[i])
            && !symbolStartsWith(text[i])
            && !isEof(text[i])
        );

        std::string_view error_text = text.substr(position, i-position);


        auto diag = diagnostics::make_diagnostic<diagnostics::InvalidCharactersDiagnostic>(diagnostics::Level::Error, std::string(error_text));
        _source_emitter.emit(error_text.begin(), diag);

        position += error_text.size();

        return addLexedToken(TokenKind::Error, byte_offset);
    }

    auto Lexer::lexSymbol(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        int32_t byte_offset { static_cast<int32_t>(position) };
        // std::cout << "lexing symbol\n";
        for (const auto& symbol : _token_spec.symbols())
        {
            if (symbol && symbol->pattern()[0] == text[position])
            {
                if (text.substr(position).starts_with(symbol->pattern()))
                {
                    position += strlen(symbol->pattern());
                    return addLexedToken(symbol->kind(), byte_offset);
                }
            }
        }

        // TODO: this will spin forever if not changed
        return addLexedToken(TokenKind::Error, byte_offset);
    }

    auto Lexer::lexKeywordOrIdentifier(std::string_view text, std::size_t& position) noexcept -> Lexer::Result
    {
        int32_t byte_offset { static_cast<int32_t>(position) };
        
        if (static_cast<unsigned char>(text[position]) > 0x7F)
        {
            // TODO: return lex error for not supporting utf8
            return Result::invalid();
        }

        if (text.substr(position).empty())
        {
            std::terminate();
        }

        std::string_view id_text = scanIdentifier(text.substr(position), 0);
        if (id_text.empty())
        {
            std::terminate();
        }

        position += id_text.length();

        if (auto keyword_kind = _token_spec.keywordKind(id_text))
        {
            return addLexedToken(keyword_kind.value(), byte_offset);
        }

        auto id = _shared_values.identifiers().insert(id_text);
        return addLexedTokenWithPayload(TokenKind::Id, byte_offset, id.index);
    }

    static auto dispatchNext(Lexer& lexer, std::string_view text, std::size_t position) -> void;


#define VIPER_DISPATCH_LEX_TOKEN(LexMethod) \
    static auto Dispatch_##LexMethod(Lexer& lexer, std::string_view text, std::size_t position) -> void \
    { \
        Lexer::Result result = lexer.LexMethod(text, position); \
        if (!result) \
        { \
        } \
        [[clang::musttail]] return dispatchNext(lexer, text, position); \
    }

    VIPER_DISPATCH_LEX_TOKEN(lexError)
    VIPER_DISPATCH_LEX_TOKEN(lexKeywordOrIdentifier)
    VIPER_DISPATCH_LEX_TOKEN(lexSymbol)
    VIPER_DISPATCH_LEX_TOKEN(lexNumericLiteral)

#define VIPER_DISPATCH_LEX_NON_TOKEN(LexMethod) \
    static auto Dispatch_##LexMethod(Lexer& lexer, std::string_view text, std::size_t position) -> void \
    { \
        lexer.LexMethod(text, position); \
        [[clang::musttail]] return dispatchNext(lexer, text, position); \
    }
VIPER_DISPATCH_LEX_NON_TOKEN(lexHorizontalWhitespace)
VIPER_DISPATCH_LEX_NON_TOKEN(lexVerticalWhitespace)


    // Create the dispatch table
    static consteval auto makeDispatchTable(TokenSpec spec) -> DispatchTableT
    {
        DispatchTableT table {};

        for (int i = 0; i < DispatchTableSize; i++)
        {
            table[i] = &Dispatch_lexError;
        }

        for (int i = 0; i < spec.idStartByteTable().size(); i++)
        {
            if (spec.idStartByteTable()[i])
            {
                table[i] = &Dispatch_lexKeywordOrIdentifier;
            }
        }
        
        for (int i = 0; i < spec.symbolStartByteTable().size(); i++)
        {
            if (spec.symbolStartByteTable()[i])
            {
                table[i] = &Dispatch_lexSymbol;
            }
        }

        for (char c = '0'; c <= '9'; c++)
        {
            table[c] = &Dispatch_lexNumericLiteral;
        }

        table[' '] = &Dispatch_lexHorizontalWhitespace;
        table['\t'] = &Dispatch_lexHorizontalWhitespace;
        table['\n'] = &Dispatch_lexVerticalWhitespace;
        table['\r'] = &Dispatch_lexVerticalWhitespace;

        return table;
    }

    static constexpr DispatchTableT DispatchTable = makeDispatchTable(Spec);

    static auto dispatchNext(Lexer& lexer, std::string_view text, std::size_t position) -> void
    {
        static int iter_count = 0;
        // std::cout << "Called: " << iter_count++ << " times" << '\n';

        // std::cout << "Lexer encountered char: " << text[position]  << " at position " << position << '\n';
        [[likely]] if (position < text.size())
        {
            // This is tail recursion to avoid growing stack while doing this loop. 
            // See https://github.com/carbon-language/carbon-lang/blob/trunk/toolchain/lex/lex.cpp
            // std::cout << "Indexing dispatch table at " << static_cast<unsigned char>(text[position]) << '\n';
            // std::cout << "Position: " << position << ". Size: " << text.size() << '\n';
            // NOLINTNEXTLINE(readability-avoid-return-with-void-value): For musttail.
            [[clang::musttail]] return DispatchTable[static_cast<unsigned char>(text[position])](lexer, text, position);
        }
    }

    auto Lexer::addLexedToken(TokenKind kind, int32_t byte_offset) noexcept -> TokenIndex
    {
        auto id = _buffer.addToken(TokenInfo(kind, byte_offset));
        return id;
    }
    
    auto Lexer::addLexedTokenWithPayload(TokenKind kind, int32_t byte_offset, int32_t payload) noexcept -> TokenIndex
    {
        auto id = _buffer.addToken(TokenInfo(kind, byte_offset, payload));
        return id;
    }

    auto Lexer::lex() && noexcept -> TokenizedBuffer
    {
        std::string_view source_text = _source.getBuffer();

        std::size_t position { 0 };
        dispatchNext(*this, source_text, position);

        _buffer.setHasErrors(_diagnostics_consumer->seen_error());

        return std::move(_buffer);
    }

    auto lex(source::SourceBuffer& source_buffer, std::weak_ptr<diagnostics::Consumer> consumer, base::SharedValues& shared_values) -> TokenizedBuffer
    {
        auto tokens = Lexer(source_buffer, consumer, shared_values, Spec).lex();
        return std::move(tokens);
    }
} // namespace viper::toolchain::lex
