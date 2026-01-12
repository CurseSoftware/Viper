#include "tests/memory_buffer_tests.h"
#include "format.h"
#include "memory_buffer.h"
#include <cstddef>

namespace viper
{
    auto bufferRawAllocateTest() -> std::optional<std::string>
    {
        constexpr std::size_t ExpectedAllocationSize = 100;
        char mem[ExpectedAllocationSize] = {};

        auto buffer = memory::MemoryBuffer::from(
            reinterpret_cast<memory::MemoryBuffer::StorageType*>(mem), 
            ExpectedAllocationSize
        );

        if (!buffer)
        {
            return "Buffer was not created";
        }

        if (buffer.value()->size() != ExpectedAllocationSize)
        {
            return format::format("Buffer size {} != expected size {}", buffer.value()->size(), ExpectedAllocationSize);
        }

        return {};
    }

    auto bufferRawAllocateTestLarge() -> std::optional<std::string>
    {
        constexpr std::size_t ExpectedAllocationSize = 100000000;
        char* mem = new char[ExpectedAllocationSize];

        auto buffer = memory::MemoryBuffer::from(
            reinterpret_cast<memory::MemoryBuffer::StorageType*>(mem), 
            ExpectedAllocationSize
        );

        if (!buffer)
        {
            return "Buffer was not created";
        }

        if (buffer.value()->size() != ExpectedAllocationSize)
        {
            return format::format("Buffer size {} != expected size {}", buffer.value()->size(), ExpectedAllocationSize);
        }

        return {};
    }
    
    auto bufferVecAllocateTestLarge() -> std::optional<std::string>
    {
        constexpr std::size_t ExpectedAllocationSize = 100000000;
        std::vector<unsigned char> data(ExpectedAllocationSize);

        auto buffer = memory::MemoryBuffer::from(std::move(data));

        if (!buffer)
        {
            return "Buffer was not created";
        }

        if (buffer.value()->size() != ExpectedAllocationSize)
        {
            return format::format("Buffer size {} != expected size {}", buffer.value()->size(), ExpectedAllocationSize);
        }

        return {};
    }
    
    auto bufferVecAllocateTest() -> std::optional<std::string>
    {
        constexpr std::size_t ExpectedAllocationSize = 100;
        std::vector<unsigned char> data(ExpectedAllocationSize);

        auto buffer = memory::MemoryBuffer::from(std::move(data));

        if (!buffer)
        {
            return "Buffer was not created";
        }

        if (buffer.value()->size() != ExpectedAllocationSize)
        {
            return format::format("Buffer size {} != expected size {}", buffer.value()->size(), ExpectedAllocationSize);
        }

        return {};
    }
    
    auto bufferSpanTest() -> std::optional<std::string>
    {
        constexpr std::size_t ExpectedAllocationSize = 100;
        std::vector<unsigned char> data(ExpectedAllocationSize);

        auto buffer = memory::MemoryBuffer::from(std::move(data));

        if (!buffer)
        {
            return "Buffer was not created";
        }

        std::span<unsigned char> span = *buffer.value();

        if (span.size() != ExpectedAllocationSize)
        {
            return format::format("Span size {} != expected size {}", span.size(), ExpectedAllocationSize);
        }

        return {};
    }

    auto bufferStringViewTest() -> std::optional<std::string>
    {
        std::string test_string = "This is my test string";

        auto buffer = memory::MemoryBuffer::from(
            reinterpret_cast<memory::MemoryBuffer::StorageType*>(test_string.data()), 
            test_string.size() 
        );

        if (!buffer)
        {
            return "Buffer was not created";
        }

        std::string_view test_string_view = buffer.value()->getBuffer();
        if (test_string_view.length() != test_string.length())
        {
            return format::format("String view length {} != string length {}", test_string_view.length(), test_string.length());
        }

        for (std::size_t i = 0; i < test_string_view.length(); i++)
        {
            if (test_string_view[i] != test_string[i])
            {
                return format::format("string_view[{}] = {} when expected is {}", i, test_string_view[i], test_string[i]);
            }
        }

        return {};
    }

    auto bufferLargeStringViewTest() -> std::optional<std::string>
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
            return "Buffer was not created";
        }

        std::string_view test_string_view = buffer.value()->getBuffer();
        if (test_string_view.length() != test_string.length())
        {
            return format::format("String view length {} != string length {}", test_string_view.length(), test_string.length());
        }

        for (std::size_t i = 0; i < test_string_view.length(); i++)
        {
            if (test_string_view[i] != test_string[i])
            {
                return format::format("string_view[{}] = {} when expected is {}", i, test_string_view[i], test_string[i]);
            }
        }

        return {};
    }
} // namespace viper
