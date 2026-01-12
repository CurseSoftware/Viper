#ifndef VIPER_COMMON_TESTS_FILESYSTEM_BUFFER_TESTS_H
#define VIPER_COMMON_TESTS_FILESYSTEM_BUFFER_TESTS_H

#include <optional>
#include <string>

namespace viper
{
    auto fileFromPath() -> std::optional<std::string>;
    auto fileToString() -> std::optional<std::string>;
    auto fileToByteVector() -> std::optional<std::string>;
} // namespace viper

#endif // VIPER_COMMON_TESTS_FILESYSTEM_BUFFER_TESTS_H
