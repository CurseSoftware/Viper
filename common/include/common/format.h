#ifndef VIPER_COMMON_FORMAT_H
#define VIPER_COMMON_FORMAT_H

#include <format>

namespace viper::format
{
    template <typename... Args>
    auto format(const std::format_string<Args...>& fmt, Args&&... args) -> std::string
    {
        return std::vformat(fmt.get(), std::make_format_args(args...));
    }
} // namespace viper::format

#endif // VIPER_COMMON_FORMAT_H
