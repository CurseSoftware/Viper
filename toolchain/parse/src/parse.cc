#include "parse.h"

namespace viper::toolchain::parse
{
	auto parse(lex::TokenizedBuffer& tokens, std::weak_ptr<diagnostics::Consumer> diagnostics_consumer) -> Tree
	{
		Tree tree{ tokens };


	}
} // namespace viper::toolchain::parse
