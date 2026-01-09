#ifndef VIPER_COMMON_UTILITY_H
#define VIPER_COMMON_UTILITY_H

#include <cstddef>
#include <cstdint>
namespace viper::utility
{

    constexpr auto hash(const char* s) -> std::uint64_t
    {
        constexpr std::uint64_t PrimeA = 54059;
        constexpr std::uint64_t PrimeB = 76963;
        constexpr std::uint64_t PrimeC = 86969;
        constexpr std::uint64_t FIRSTH = 37;

        std::uint64_t hash = FIRSTH;
        while (*s)
        {
            hash = (hash * PrimeA) ^ (s[0] * PrimeB);
            s++;
        }

        return hash;
    }
    
    consteval auto static_hash(const char* s) -> std::uint64_t
    {
        return hash(s);
    }
} // namespace viper::utility

#endif // VIPER_COMMON_UTILITY_H
