#ifndef VIPER_COMMON_LOGGER_H
#define VIPER_COMMON_LOGGER_H

#include "emit.h"
#include "format.h"
#include <iostream>



namespace viper::cli
{
    template<typename ...Args>
    auto print(const std::format_string<Args...>& fmt, Args&&... args)
    {
        emit(std::cout, format::format(fmt, args...));
    }
    
    template<typename ...Args>
    auto println(const std::format_string<Args...>& fmt, Args&&... args)
    {
        emit(std::cout, format::format(fmt, args...) + '\n');
    }
} // namespace viper::cli

#endif // VIPER_COMMON_LOGGER_H
