#ifndef VIPER_PARSE_STATE_H
#define VIPER_PARSE_STATE_H

#include "lex/tokenized_buffer.h"
#include <cstdint>

namespace viper::toolchain::parse
{
    struct ParseState
    {
        public:
            enum class Kind
            {
                FunctionIntroducer,
                Identifier,
                ParameterList,
                FunctionReturn,
                CodeBlock,
            } kind;

            lex::TokenIndex token_index;

            std::int32_t subtree_end { 0 };
        
        public:
#define ParseNodeBuildMethod(Kind) \
            [[nodiscard]] static auto Kind(lex::TokenIndex token_index) -> ParseState \
            {\
                return ParseState {\
                    .kind = Kind::Kind,\
                    .token_index = token_index, \
                };\
            }

            // NOLINTNEXTLINE
            [[nodiscard]] static auto FunctionIntroducer(lex::TokenIndex token_index) -> ParseState
            {
                return ParseState {
                    .kind = Kind::FunctionIntroducer,
                    .token_index = token_index,
                };
            }
            
            // NOLINTNEXTLINE
            [[nodiscard]] static auto Identifier(lex::TokenIndex token_index) -> ParseState
            {
                return ParseState {
                    .kind = Kind::Identifier,
                    .token_index = token_index,
                };
            }
            
            // NOLINTNEXTLINE
            [[nodiscard]] static auto ParameterList(lex::TokenIndex token_index) -> ParseState
            {
                return ParseState {
                    .kind = Kind::ParameterList,
                    .token_index = token_index,
                };
            }
            
            // NOLINTNEXTLINE
            [[nodiscard]] static auto FunctionReturn(lex::TokenIndex token_index) -> ParseState
            {
                return ParseState {
                    .kind = Kind::FunctionReturn,
                    .token_index = token_index,
                };
            }
            
            // NOLINTNEXTLINE
            [[nodiscard]] static auto CodeBlock(lex::TokenIndex token_index) -> ParseState
            {
                return ParseState {
                    .kind = Kind::CodeBlock,
                    .token_index = token_index,
                };
            }
    };
} // namespace viper::toolchain::parse

#endif // VIPER_PARSE_STATE_H
