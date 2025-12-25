#ifndef VIPER_TOOLCHAIN_BASE_LOG_H
#define VIPER_TOOLCHAIN_BASE_LOG_H

#include "common/console.h"

namespace viper::toolchain::base::log
{
    template<typename ...Args>
    auto info(const std::format_string<Args...>& fmt, Args&&... args)
    {
        cli::println(fmt, args...);
    }
} // namespace viper::toolchain::base

#endif // VIPER_TOOLCHAIN_BASE_LOG_H
