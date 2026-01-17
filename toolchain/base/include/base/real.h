#ifndef VIPER_TOOLCHAIN_BASE_REAL_H
#define VIPER_TOOLCHAIN_BASE_REAL_H

#include "base/canonical_values.h"
#include "base/string_literal_store.h"
#include <cstdint>
namespace viper::toolchain::base
{
    struct Real
    {
        int64_t mantissa;
        int64_t exponent;
    };
} // namespace viper::toolchain::base

#endif // VIPER_TOOLCHAIN_BASE_REAL_H
