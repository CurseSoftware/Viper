#ifndef VIPER_TOOLCHAIN_LEX_LEXER_H
#define VIPER_TOOLCHAIN_LEX_LEXER_H

#include "diagnostics/consumer.h"
#include "source/source_buffer.h"
#include "token_kind.h"
#include "tokenized_buffer.h"
#include "tokens.h"
#include <string_view>
#include <utility>

namespace viper::toolchain::lex
{
    class Lexer
    {
        public:
            class [[nodiscard]] Result
            {
                public:
                    explicit Result()
                        : _valid_token_formed{ true }
                    {}

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
            {}

        // Methods
        public:
            auto lex() && noexcept -> TokenizedBuffer;

            auto lexError(std::string_view text, std::size_t& position) noexcept -> Result;

            auto lexKeywordOrIdentifier(std::string_view text, std::size_t& position) noexcept -> Result;

            auto lexHorizontalWhitespace(std::string_view text, std::size_t& position) noexcept -> Result;
            
            auto lexVerticalWhitespace(std::string_view text, std::size_t& position) noexcept -> Result;

        // Private Methods
        private:
            [[nodiscard]] auto scanIdentifier(std::string_view text, std::size_t i) -> std::string_view;

            [[nodiscard]] auto skipHorizontalWhitespace(std::string_view text, std::size_t i) -> std::size_t;

            [[nodiscard]] auto skipVerticalWhitespace(std::string_view text, std::size_t i) -> std::size_t;

            auto addLexedToken(TokenKind kind) noexcept -> void;
        
        private:
            source::SourceBuffer& _source;
            std::weak_ptr<diagnostics::Consumer> _diagnostics_consumer;
            const TokenSpec& _token_spec;
            TokenizedBuffer _buffer;
    };
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_LEXER_H
