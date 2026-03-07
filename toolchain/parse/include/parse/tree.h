#ifndef VIPER_TOOLCHAIN_PARSE_TREE_H
#define VIPER_TOOLCHAIN_PARSE_TREE_H

#include "common/containers/store.h"
#include "lex/tokenized_buffer.h"
#include "node.h"
#include <memory>

namespace viper::toolchain::parse
{
    class ParseNodeIndex : public containers::Index<ParseNode> 
    {
        using Index::index;
        static const ParseNodeIndex None;
    };

    class Tree
    {
        // Special Members
        public:
			// Links the tree to the tokenized buffer. The `parse::parse` function is responsible 
            // for calling this constructor and generating the tree from the tokenized buffer.
            [[nodiscard]] explicit Tree(lex::TokenizedBuffer& tokens) : _tokens{ tokens } {}

            /* Not Copyable */
            Tree(const Tree&) noexcept = delete;
            auto operator=(const Tree&) noexcept -> Tree& = delete;

            /* Movable */
            Tree(Tree&&) noexcept = default;
            auto operator=(Tree&&) noexcept -> Tree& = default;

        // API
        public:
            template <typename ... Args>
            auto emplaceNode(Args&&... args) -> void
            {
                _parse_nodes.emplace(args);
            }

			// Returns `true` if the tree has any errors. 
			[[nodiscard]] auto has_errors() const noexcept -> bool { return _has_errors; }

            // Set whether the tree contains errors.
			auto set_has_errors(bool has_errors) noexcept -> void { _has_errors = has_errors; }

        // Private members
        private:
			// The tokenized buffer that the tree was generated from
			lex::TokenizedBuffer& _tokens;

            // Linearized buffer for the parse nodes. The Tree class provides a convenient API around this
            containers::Store<ParseNodeIndex, ParseNode> _parse_nodes{};

            bool _has_errors{ false };
    };
} // namespace viper::toolchain::parse

#endif // VIPER_TOOLCHAIN_PARSE_TREE_H
