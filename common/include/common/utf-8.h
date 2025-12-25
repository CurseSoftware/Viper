#ifndef VIPER_COMMON_UTF_8_H
#define VIPER_COMMON_UTF_8_H

#include "memory_buffer.h"

#include <cstdint>
#include <span>

namespace viper::encoding::utf8
{
    // Get the number of bytes that should be read from this sequence
    // 0xxxxxxx: 1-byte sequence (ASCII)
    // 110xxxxx: 2-byte sequence
    // 1110xxxx: 3-byte sequence
    // 11110xxx: 4-byte sequence
    auto getNumBytesInCharacterSequence(std::span<memory::MemoryBuffer::StorageType> buffer, std::size_t offset) -> int;
    
    // TODO: move to different file?
    auto getBitAt(std::uint8_t byte, std::size_t offset) noexcept -> int;
    
    // Return `true` if a file has the UTF-8 byte order mark. `false` otherwise.
    // Most files on Linux/MacOS will not have this.
    auto hasByteOrderMark(std::span<memory::MemoryBuffer::StorageType> buffer) noexcept -> bool;


    // Read a 1 byte character from `buffer` at the offset `offset`.
    // This function checks that the returned character is a valid 1-byte ascii utf-8 character
    auto readExpect1ByteChar(std::span<memory::MemoryBuffer::StorageType> buffer, std::size_t offset) noexcept -> std::optional<char8_t>;


} // namespace viper::encoding::utf8

#endif // VIPER_COMMON_UTF_8_H
