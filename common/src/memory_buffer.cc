#include "memory_buffer.h"
#include <cstdlib>
#include <iostream>
#include <memory>

namespace viper::memory
{
    auto MemoryBuffer::from(MemoryBuffer::StorageType* data, MemoryBuffer::SizeType length) noexcept -> std::optional<std::unique_ptr<MemoryBuffer>>
    { 
        try
        {
            return std::unique_ptr<MemoryBuffer>(new MemoryBuffer(data, length));
        } catch (...)
        {
            // Usually an allocation error
            return {};
        }
    }
    
    auto MemoryBuffer::from(std::vector<StorageType>&& data) noexcept -> std::optional<std::unique_ptr<MemoryBuffer>>
    { 
        try
        {
            return std::unique_ptr<MemoryBuffer>(new MemoryBuffer(data.data(), data.size()));
        } catch (...)
        {
            // Usually an allocation error
            return {};
        }
    }

    MemoryBuffer::MemoryBuffer(MemoryBuffer::StorageType* in_data, MemoryBuffer::SizeType length)
    {
        _length = length;
        _data = new StorageType[length] {};
        std::memcpy(_data, in_data, length);
    }

    /*
     * Special Member Definitions
     *
     */
    
    // Move constructor
    MemoryBuffer::MemoryBuffer(MemoryBuffer&& other) noexcept
        : _data{ std::exchange(other._data, nullptr) }
        , _length{ std::exchange(other._length, 0) }
    {}

    // Move assignment
    auto MemoryBuffer::operator=(MemoryBuffer&& other) noexcept -> MemoryBuffer&
    {
        if (this != &other)
        {
            delete[] _data;

            _data = std::exchange(other._data, nullptr);
            _length = std::exchange(other._length, 0);
        }
        
        return *this;
    }

    // Destructor
    MemoryBuffer::~MemoryBuffer() noexcept
    {
        std::cout << "Deleting data\n";
        delete[] _data;
    }


} // namespace viper::memory
