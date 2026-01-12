#ifndef VIPER_COMMON_TESTS_MEMORY_BUFFER_TESTS_H
#define VIPER_COMMON_TESTS_MEMORY_BUFFER_TESTS_H
#include <optional>
#include <string>

namespace viper
{
    auto bufferRawAllocateTest() -> std::optional<std::string>;

    auto bufferRawAllocateTestLarge() -> std::optional<std::string>;
    
    auto bufferVecAllocateTestLarge() -> std::optional<std::string>;
    
    auto bufferVecAllocateTest() -> std::optional<std::string>;
    
    auto bufferSpanTest() -> std::optional<std::string>;

    auto bufferStringViewTest() -> std::optional<std::string>;

    auto bufferLargeStringViewTest() -> std::optional<std::string>;
} // namespace viper

#endif // VIPER_COMMON_TESTS_MEMORY_BUFFER_TESTS_H
