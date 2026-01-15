#ifndef VIPER_TOOLCHAIN_PARSE_NODE_H
#define VIPER_TOOLCHAIN_PARSE_NODE_H

#include <cstdint>
namespace viper::toolchain::parse
{
    // The base class for parse nodes
    template <typename T>
    class Node
    {
        public:
            // The number of children that node of type T must have
            static constexpr auto childCount() -> int32_t { return T::child_count; }

            // Returns `true` if node of type T has a fixed amount of children
            static constexpr auto hasFixedChildCount() -> bool { return T::has_fixed_child_count; }

        private:
            
    };

} // namespace viper::toolchain::parse

#endif // VIPER_TOOLCHAIN_PARSE_NODE_H
