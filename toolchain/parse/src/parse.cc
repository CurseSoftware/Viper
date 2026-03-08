#include "parse.h"
#include "context.h"

namespace viper::toolchain::parse
{
	auto parse(lex::TokenizedBuffer& tokens, std::weak_ptr<diagnostics::Consumer> diagnostics_consumer) -> Tree
	{
        ParseContext parse_context{ tokens };
        auto tree = parse_context.parse();

        return tree;
	}
} // namespace viper::toolchain::parse
