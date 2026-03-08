#include "context.h"

namespace viper::toolchain::parse
{
    auto ParseContext::parse() noexcept -> Tree
    {
        return std::move(_tree);
    }

    auto ParseContext::parseFile() noexcept -> void
    {
        
    }
} // namespace viper::toolchain::parse
