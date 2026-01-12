#ifndef VIPER_COMMON_TESTS_MEMORY_BUFFER_TESTS_H
#define VIPER_COMMON_TESTS_MEMORY_BUFFER_TESTS_H

namespace viper
{
    auto bufferRawAllocateTest() -> bool;

    auto bufferRawAllocateTestLarge() -> bool;
    
    auto bufferVecAllocateTestLarge() -> bool;
    
    auto bufferVecAllocateTest() -> bool;
    
    auto bufferSpanTest() -> bool;

    auto bufferStringViewTest() -> bool;

    auto bufferLargeStringViewTest() -> bool;
} // namespace viper

#endif // VIPER_COMMON_TESTS_MEMORY_BUFFER_TESTS_H
