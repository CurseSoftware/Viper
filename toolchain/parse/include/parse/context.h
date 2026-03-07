#ifndef VIPER_PARSE_CONTEXT_H
#define VIPER_PARSE_CONTEXT_H

#include <vector>

namespace viper::toolchain::parse
{
	struct ContextNode
	{
	};

	class Context
	{
		// Special members
		public:
			[[nodiscard]] explicit Context() noexcept
			{
			}

		// API
		public:
			auto openNode(ContextNode node) -> void
			{
				_open_node_stack.push_back(std::move(node));
			}

			auto closeNode() -> ContextNode
			{
				return std::move(_open_node_stack.pop_back());
			}

		private:
			// A stack of currently open nodes
			std::vector<ContextNode> _open_node_stack{};
	};
} // namespace viper::toolchain::parse

#endif // VIPER_PARSE_CONTEXT_H