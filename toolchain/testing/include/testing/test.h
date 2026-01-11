#ifndef VIPER_TOOLCHAIN_TESTING_TEST_H
#define VIPER_TOOLCHAIN_TESTING_TEST_H

#include <functional>
namespace viper::toolchain::testing
{
    using TestFunction = std::function<auto()->bool>;
}

#endif // VIPER_TOOLCHAIN_TESTING_TEST_H
