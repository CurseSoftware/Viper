#include "tests/memory_buffer_tests.h"
#include "memory_buffer.h"
#include <cstddef>

namespace viper
{
    auto bufferRawAllocateTest() -> bool
    {
        constexpr std::size_t ExpectedAllocationSize = 100;
        char mem[ExpectedAllocationSize] = {};

        auto buffer = memory::MemoryBuffer::from(
            reinterpret_cast<memory::MemoryBuffer::StorageType*>(mem), 
            ExpectedAllocationSize
        );

        if (!buffer)
        {
            return false;
        }

        return buffer.value()->size() == ExpectedAllocationSize;
    }

    auto bufferRawAllocateTestLarge() -> bool
    {
        constexpr std::size_t ExpectedAllocationSize = 100000000;
        char* mem = new char[ExpectedAllocationSize];

        auto buffer = memory::MemoryBuffer::from(
            reinterpret_cast<memory::MemoryBuffer::StorageType*>(mem), 
            ExpectedAllocationSize
        );

        if (!buffer)
        {
            return false;
        }

        return buffer.value()->size() == ExpectedAllocationSize;
    }
    
    auto bufferVecAllocateTestLarge() -> bool
    {
        constexpr std::size_t ExpectedAllocationSize = 100000000;
        std::vector<unsigned char> data(ExpectedAllocationSize);

        auto buffer = memory::MemoryBuffer::from(std::move(data));

        if (!buffer)
        {
            return false;
        }

        return buffer.value()->size() == ExpectedAllocationSize;
    }
    
    auto bufferVecAllocateTest() -> bool
    {
        constexpr std::size_t ExpectedAllocationSize = 100;
        std::vector<unsigned char> data(ExpectedAllocationSize);

        auto buffer = memory::MemoryBuffer::from(std::move(data));

        if (!buffer)
        {
            return false;
        }

        return buffer.value()->size() == ExpectedAllocationSize;
    }
    
    auto bufferSpanTest() -> bool
    {
        constexpr std::size_t ExpectedAllocationSize = 100;
        std::vector<unsigned char> data(ExpectedAllocationSize);

        auto buffer = memory::MemoryBuffer::from(std::move(data));

        if (!buffer)
        {
            return false;
        }

        std::span<unsigned char> span = *buffer.value();

        return span.size() == ExpectedAllocationSize;
    }

    auto bufferStringViewTest() -> bool
    {
        std::string test_string = "This is my test string";

        auto buffer = memory::MemoryBuffer::from(
            reinterpret_cast<memory::MemoryBuffer::StorageType*>(test_string.data()), 
            test_string.size() 
        );

        if (!buffer)
        {
            return false;
        }

        std::string_view test_string_view = buffer.value()->getBuffer();
        if (test_string_view.length() != test_string.length())
        {
            return false;
        }

        for (std::size_t i = 0; i < test_string_view.length(); i++)
        {
            if (test_string_view[i] != test_string[i])
            {
                return false;
            }
        }

        return true;
    }

    auto bufferLargeStringViewTest() -> bool
    {
        std::string test_string;

        for (std::size_t i = 0; i < 10000000; i++)
        {
            test_string.push_back(static_cast<char>(i % 256));
        }

        auto buffer = memory::MemoryBuffer::from(
            reinterpret_cast<memory::MemoryBuffer::StorageType*>(test_string.data()), 
            test_string.size() 
        );

        if (!buffer)
        {
            return false;
        }

        std::string_view test_string_view = buffer.value()->getBuffer();
        if (test_string_view.length() != test_string.length())
        {
            return false;
        }

        for (std::size_t i = 0; i < test_string_view.length(); i++)
        {
            if (test_string_view[i] != test_string[i])
            {
                return false;
            }
        }

        return true;
    }
} // namespace viper
