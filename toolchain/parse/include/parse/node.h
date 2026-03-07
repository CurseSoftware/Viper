#ifndef VIPER_TOOLCHAIN_PARSE_NODE_H
#define VIPER_TOOLCHAIN_PARSE_NODE_H

#include <cstdint>

#include "lex/tokenized_buffer.h"
#include "node_kind.h"

namespace viper::toolchain::parse
{
	class ParseNode
	{
		// Special members
		public:
			[[nodiscard]] explicit ParseNode(NodeKind kind, lex::TokenIndex index) noexcept

				: _kind{ kind }
				, _token_index{ index }
			{}

		private:
			// The kind of parse node this is
			NodeKind _kind;

			// The token that this parse node references
			lex::TokenIndex _token_index;
	};
} // namespace viper::toolchain::parse

#endif // VIPER_TOOLCHAIN_PARSE_NODE_H
