#ifndef VIPER_COMMON_EMIT_H
#define VIPER_COMMON_EMIT_H

#include <ostream>
namespace viper
{
    // Emit a string to the specified `ostream`
    auto emit(std::ostream& out, std::string_view data) -> void;
} // namespace viper

#endif // VIPER_COMMON_EMIT_H
