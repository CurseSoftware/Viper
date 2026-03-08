#ifndef VIPER_PARSE_CONTEXT_H
#define VIPER_PARSE_CONTEXT_H

#include "lex/tokenized_buffer.h"
#include "state.h"
#include "tree.h"
#include <vector>

namespace viper::toolchain::parse
{
	class ParseContext
	{
		// Special members
		public:
			[[nodiscard]] explicit ParseContext(
                lex::TokenizedBuffer& tokens
            ) noexcept 
                : _tokens{ tokens }
                , _tree{ Tree{ tokens } }
                , _position{ tokens.tokens().indices().begin() }
            {}

		// API
		public:
            // Push the new state for the kind of node that we are parsing
            auto pushState(ParseState state) noexcept -> void
            {
                _state_stack.push_back(state);
            }

            // Pops the state from the stack and returns it.
            // This is mainly used for setting when the tree 
            // boundaries are for nodes.
            [[nodiscard]] auto popState() noexcept -> ParseState
            {
                auto state = _state_stack.back();
                _state_stack.pop_back();
                return state;
            }

            [[nodiscard]] auto parse() noexcept -> Tree;

        // Parse functions
        public:
            auto parseFile() noexcept -> void;

            auto consumeChecked(lex::TokenKind kind) noexcept -> lex::TokenIndex;
        
            auto consume() noexcept -> lex::TokenIndex;

            auto currentPositionKind() const noexcept -> lex::TokenKind;

            [[nodiscard]] auto currentPositionIs(lex::TokenKind kind) const noexcept -> bool;

		private:
            // The stack of `ParseState` nodes to track what we are currently parsing.
            // This is useful for constructing the tree in pre-order.
            std::vector<ParseState> _state_stack {};

            const lex::TokenizedBuffer& _tokens;

            lex::TokenizedBuffer::Iterator _position;

            Tree _tree;
	};
} // namespace viper::toolchain::parse

#endif // VIPER_PARSE_CONTEXT_H
