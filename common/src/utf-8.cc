#include "utf-8.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

namespace viper::encoding::utf8
{
    // TODO: further test this
    auto hasByteOrderMark(std::span<memory::MemoryBuffer::StorageType> buffer) noexcept -> bool
    {
        // const auto* data = buffer.data();
        std::array<unsigned char, 3> bom = {
            static_cast<unsigned char>(buffer[0]),
            static_cast<unsigned char>(buffer[1]),
            static_cast<unsigned char>(buffer[2])
        };

        return (
            bom[0] == 0xEF 
            && bom[1] == 0xBB 
            && bom[2] == 0xBF
        );
    }

    auto getNumBytesInCharacterSequence(std::span<memory::MemoryBuffer::StorageType> buffer, std::size_t offset) -> int
    {
        auto byte = static_cast<std::uint8_t>(buffer[offset]);

        // 0xxx_xxxx
        if (getBitAt(byte, 7) == 0)
        {
            return 1;
        }
        
        // 110x_xxxx
        if (getBitAt(byte, 7) == 1 && getBitAt(byte, 6) == 1 && getBitAt(byte, 5) == 0)
        {
            return 2;
        }
        
        // 1110_xxxx
        if (
            getBitAt(byte, 7) == 1 
            && getBitAt(byte, 6) == 1 
            && getBitAt(byte, 5) == 1 
            && getBitAt(byte, 4) == 0
        ) {
            return 3;
        }
        
        // 1111_0xxx
        if (
            getBitAt(byte, 7) == 1 
            && getBitAt(byte, 6) == 1 
            && getBitAt(byte, 5) == 1 
            && getBitAt(byte, 4) == 1
            && getBitAt(byte, 3) == 0
        ) {
            return 3;
        }

        // Likely calling this on a continuation byte. This case should be handled.
        return -1;
    }

    auto readExpect1ByteChar(std::span<memory::MemoryBuffer::StorageType> buffer, std::size_t offset) noexcept -> std::optional<char8_t>
    {
        if (getNumBytesInCharacterSequence(buffer, offset) != 1)
        {
            return std::nullopt;
        }

        return buffer[offset-1];
    }

    auto getBitAt(std::uint8_t byte, std::size_t offset) noexcept -> int
    {
        return (byte >> offset) & 1;
    }
} // namespace viper::encoding::utf8
