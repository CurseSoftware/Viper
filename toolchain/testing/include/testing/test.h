#ifndef VIPER_TOOLCHAIN_TESTING_TEST_H
#define VIPER_TOOLCHAIN_TESTING_TEST_H

#include <functional>
#include <optional>
namespace viper::toolchain::testing
{
    using TestFunction = std::function<auto()->std::optional<std::string>>;
}

#endif // VIPER_TOOLCHAIN_TESTING_TEST_H
