#ifndef VIPER_TOOLCHAIN_LEX_LEXER_H
#define VIPER_TOOLCHAIN_LEX_LEXER_H

#include "characters.h"
#include "diagnostics/consumer.h"
#include "diagnostics/emitter.h"
#include "source/source_buffer.h"
#include "token_kind.h"
#include "tokenized_buffer.h"
#include "tokens.h"
#include <memory>
#include <string_view>
#include <utility>

namespace viper::toolchain::lex
{
    // This is an emitter of LocationType `const char *`
    // The location type is a pointer to a location within
    // the source buffer
    class SourcePointerEmitter : public diagnostics::Emitter<const char*>
    {
        public:
            explicit SourcePointerEmitter(std::weak_ptr<diagnostics::Consumer> consumer, source::SourceBuffer& source)
                : diagnostics::Emitter<const char*>(consumer)
                , _source{ source }
            {}

            auto convertLocation(const char* location) const noexcept -> diagnostics::ConvertedLocation override
            {
                auto text = _source.getBuffer();
                int32_t offset { static_cast<int32_t>(location - text.begin()) };
               
                // Find the beginning of the line for this location
                int32_t line_begin_offset { offset };
                while (!isVerticalWhitespace(text[offset]))
                {
                    line_begin_offset--;
                }

                // Find the end of the line
                int32_t line_end_offset { offset };
                while (!isVerticalWhitespace(text[offset]))
                {
                    line_end_offset--;
                }

                std::string_view file_name = _source.filepath();
                int32_t column_number { offset - line_begin_offset };
                int32_t line_number { 0 };
                int32_t length = 1;
                std::string_view line = text.substr(line_begin_offset, line_end_offset);

                return diagnostics::ConvertedLocation {
                    .location = diagnostics::Location(
                        file_name,
                        line,
                        line_number,
                        column_number,
                        length
                    )
                };
            }

        private:
            source::SourceBuffer& _source;
    };

    class Lexer
    {
        public:
            class [[nodiscard]] Result
            {
                public:
                    // This is mainly used for tail recursion
                    explicit(false) Result(TokenIndex discarded_index)
                        : _valid_token_formed{ true }
                    { (void)discarded_index; }

                    explicit Result(bool valid)
                        : _valid_token_formed{ valid }
                    {}

                    static auto invalid() -> Result { return Result(false); }

                    explicit operator bool() const { return _valid_token_formed; }
                
                private:
                    bool _valid_token_formed;
            };

        // Special Members
        public:
            [[nodiscard]] explicit Lexer(source::SourceBuffer& source, std::weak_ptr<diagnostics::Consumer> diagnostics_consumer, const TokenSpec& token_spec)
                : _source{ source }
                , _diagnostics_consumer{ std::move(diagnostics_consumer) }
                , _token_spec{ token_spec }
                , _source_emitter{ diagnostics_consumer, source }
            {}

        // Methods
        public:
            auto lex() && noexcept -> TokenizedBuffer;

        // Token lexer methods
        public:
            auto lexError(std::string_view text, std::size_t& position) noexcept -> Result;
            auto lexKeywordOrIdentifier(std::string_view text, std::size_t& position) noexcept -> Result;
            auto lexSymbol(std::string_view text, std::size_t& position) noexcept -> Result;
            auto lexNumericLiteral(std::string_view text, std::size_t& position) noexcept -> Result;

        // Non-token lexer methods
        public:
            auto lexHorizontalWhitespace(std::string_view text, std::size_t& position) noexcept -> void;
            auto lexVerticalWhitespace(std::string_view text, std::size_t& position) noexcept -> void;

        // Private Methods
        private:
            [[nodiscard]] auto scanIdentifier(std::string_view text, std::size_t i) -> std::string_view;

            [[nodiscard]] auto skipHorizontalWhitespace(std::string_view text, std::size_t i) -> std::size_t;

            [[nodiscard]] auto skipVerticalWhitespace(std::string_view text, std::size_t i) -> std::size_t;

            auto addLexedToken(TokenKind kind) noexcept -> TokenIndex;
        
        private:
            source::SourceBuffer& _source;
            std::weak_ptr<diagnostics::Consumer> _diagnostics_consumer;
            const TokenSpec& _token_spec;
            TokenizedBuffer _buffer;
            SourcePointerEmitter _source_emitter;
    };
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_LEXER_H
